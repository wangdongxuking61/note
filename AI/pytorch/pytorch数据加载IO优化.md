深度学习的训练过程中，数据集通常很大，**数据加载**可能成为性能的瓶颈，尤其是当数据是以小文件存在的时候。这里以**PyTorch**读取**ImageNet2012**的数据为例（共1281167张图片），总结了提高**数据加载**性能的几种方法。训练时，数据的流向是：**硬盘 -> 内存 -> 显存**，不管任何的数据，都可以从这两个流向中想些办法。

## 1、使用PIL-SIMD或accimage替换PIL

这个方法可以提高单张图片的读取速度。那是因为训练时，数据加载的耗时主要集中在从**硬盘**读数据到**内存**，最常使用的python库是[Pillow](https://github.com/python-pillow/Pillow)来实现的，所以能不能使用更快的库来替代Pillow呢？

[PyTrorch官方已经早就考虑过了](https://github.com/pytorch/vision)，下面是官方原文（github Torchvision项目）：

```
Torchvision currently supports the following image backends:

- Pillow (default)
- Pillow-SIMD - a much faster drop-in replacement for Pillow with SIMD. If installed will be used as the default.
- accimage - if installed can be activated by calling torchvision.set_image_backend('accimage')
```

### 结论

先讲结论：在相同条件下，accimage和PIL-SIMD比PIL快1~2倍。

[PIL-SIMD原理](https://github.com/uploadcare/pillow-simd)：PIL-SIMD是PIL的升级版，完全兼容PIL，所以python代码不用修改。PIL-SIMD只是使用了CPU的SIMD( single instruction, multiple data)指令来替代原来的SISD指令 。

[accimage原理](https://github.com/pytorch/accimage)：前面的PIL-SIMD和PIL，都是从硬盘读进来图片，存为PIL.Image.Image对象。accimage模式会将图片读进来存为accimage对象，再转为Tensor。

###  Pillow-SIMD

只需要卸载和安装几个库即可，python代码不用动。

- 安装依赖库zlib

  ```bash
  git clone https://github.com/madler/zlib.git
  cd zlib
  ./configure
  make test
  make install
  ```

- 安装依赖库zlib

  ```bash
  wget http://www.ijg.org/files/jpegsrc.v9c.tar.gz
  tar -xf jpegsrc.v9c.tar.gz
  cd jpeg-9c
  ./configure
  make
  make test
  make install
  ```

- 改变环境变量`LD_LIBRARY_PATH`，否则会报错`ImportError: libjpeg.so.9: cannot open shared object file: No such file or directory `

  ```bash
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
  ```

- 安装Pillow-SIMD

  ```
  pip uninstall Pillow
  pip install Pillow-SIMD
  ```

### accimage

- 安装Intel IPP：在这个[链接](https://registrationcenter.intel.com/en/products/postregistration/?sn=nvgw-9hhjlkk7&EmailID=wangdongxuking61@gmail.com&Sequence=2289436&dnld=t)下载Intel IPP，下载的文件名是l_ipp_2018.3.222.tgz，放到linux上解压后安装

  ```bash
  tar -xf l_ipp_2018.3.222.tgz
  cd l_ipp_2018.3.222
  ./install.sh
  ```

- 安装ibjpeg-turbo：在这个[链接](https://sourceforge.net/projects/libjpeg-turbo/files/2.0.0/ )下载，ubuntu下载[libjpeg-turbo-official_2.0.0_amd64.deb](https://sourceforge.net/projects/libjpeg-turbo/files/2.0.0/libjpeg-turbo-official_2.0.0_amd64.deb/download)，centos下载对应的rpm文件。ubuntu下会安装到`/opt/libjpeg-turbo/ `

  ```bash
  dpkg -i libjpeg-turbo-official_2.0.0_amd64.deb
  ```

- 编译安装accimage

  ```bash
  git clone https://github.com/pytorch/accimage.git
  cd accimage
  ```

  改setup.py文件：

  ```bash
  # 第53行的
  jpeg_turbo_root = '/usr/local/opt/jpeg-turbo'
  # 替换为
  jpeg_turbo_root = '/opt/libjpeg-turbo/'
  
  # 第57、58行
  'lib_dir':  join('/usr/local/opt/jpeg-turbo', 'lib'),
  'include_dir': join('/usr/local/opt/jpeg-turbo', 'include'),
  # 改为
  'lib_dir': join(jpeg_turbo_root, 'lib64'),
  'include_dir': join(jpeg_turbo_root, 'include'),
  ```

  再修改两个地方：

  ```bash
  # 改/opt/libjpeg-turbo/jconfig.h，注释下面这句
  #define HAVE_STDLIB_H
  
  # 把/opt/libjpeg-turbo/lib64:/opt/intel/ipp/lib/intel64添加到LD_LIBRARY_PATH 环境变量（否则python会报错找不到.so）
  export LD_LIBRARY_PATH=/opt/libjpeg-turbo/lib64:/opt/intel/ipp/lib/intel64:$LD_LIBRARY_PATH
  ```

  安装python-dev，防止出现错误：`fatal error: Python.h: No such file or directory`

  ```
  sudo apt-get install python-dev   # for python2.x installs
  sudo apt-get install python3-dev  # for python3.x installs
  ```

  在accimage文件夹下执行

  ```bash
  pip install scipy numpy
  ./test.sh
  ```

  如果要使用accimage，在python代码中要加入下面语句

  ```python
  import accimage
  torchvision.set_image_backend('accimage')
  ```

## 2、多进程异步读取

可以将**硬盘 -> 内存**这个过程做成多进程异步加载：即开启很多个读数据进程，每个进程每次只读取一个batch的数据，读取后放到一个共享数据池中，数据池满就不再读取，数据池不满，就分配给进程读取batch的任务。而训练的每次迭代中，从数据池中拿1个batch，只要数据池不空，就可以做到数据读取的0耗时。

具体可参考`PyTorch`的`DataLoader`类。

如果要了解PyTorch的数据加载过程，可见[这篇文章](https://github.com/wangdongxuking61/AI_note/wiki/PyTorch%E6%95%B0%E6%8D%AE%E5%8A%A0%E8%BD%BD%E6%95%99%E7%A8%8B)

### 代码分析 - DataLoader类

DataLoader类在`torch/utils/data/dataloader.py`文件中

以github项目`pytorch/example`中`imagenet/main_amp.py`为例讲解使用`dataloader`读取图片。

#### 数据流图

- 场景：单机8块gpu卡，DDP模式，会开启8个主进程，每个主进程开启4个数据读取进程（以下简称worker进程）

- 总括：如下图所示，中间的SimpleQueue相当于一个池子，左边的四个worker进程相当于4个工人，不断地创造一个个batch往池子里放，右边的主进程每进行一次迭代都要从池子里取走一个batch去进行计算。只要左边`生产`的速度大于右边`消费`的速度，即在右边看来，每次要取走1个batch的时候，它都被准备好了，数据加载就不是瓶颈。

  ![](<https://i.loli.net/2019/05/13/5cd92043662bf81507.jpg>)

- worker进程时间分析：以`batchsize=128`为例，一个worker进程读取一个`batch`耗时`1.7s。`理论分析可得，如果4个worker完全并行执行，平均读一个`batch`耗时`1.7s/4=425ms`，如果换成3个worker则耗时`1.7s/3=566ms`。经过实测，真实时间是440ms和580ms，和理论分析基本一致。

- 主进程时间分析：以`resnet50`为例，每次迭代中，先查看内存上是否有1个batch的数据准备好，没有就等待，有的话就加载到gpu（和前向反向相比，这里的时间可忽略之），前向时间`fp=224ms`，反正计算时间`bp=430ms`，则一次迭代的计算时间是`fp+bp=654ms`。

- 分析：如果使用3个worker进程，平均读一个batch时间为580ms，基本覆盖了计算时间654ms，如果换成4个worker，平均读一个batch时间为440ms，则可以完全覆盖计算时间。经过实测也是，3个worker勉强够用，数据加载还是要少许时间的，换成4个worker则完全没有数据加载的等待，效果不错。

- 结论：这个场景下，一个主进程开启>=4个worker进程就没有数据加载的等待。

#### 代码详解

首先，main_amp.py中取数据的代码就一句：

```python
for i, (input, target) in enumerate(train_loader):
	# 前向计算、反向计算、更新梯度
```

这句能翻译成三句话：

```python
# 会返回一个_DataLoaderIter对象，生成过程中会执行_DataLoaderIter.__init__，会生成4个worker进程，进程的执行入口是_worker_loop。还会生成一个线程，线程的执行入口是_worker_manager_loop，用于pin_memory操作
it = iter(train_loader)

for i in range(len(train_loader))
	# 这里的next函数会调用_DataLoaderIter.next()
	input, target = next(it)
    
    # 前向计算、反向计算、更新梯度
```

![](https://i.loli.net/2019/05/13/5cd93634b6d2c65873.jpg)

_worker_loop进程内部执行以下操作：

```python
DatasetFolder.__getitem__(index=0)
	path, target = self.samples[0]  # path=n01440764/n01440764_10026.JPEG
	Read ssd(10ms)：从硬盘读图片到内存(path -> PIL.Image.Image)
	Transform(2ms)：PIL.Image.Image -> Torch.tensor

DatasetFolder.__getitem__(index=1)
	path, target = self.samples[1]  # path=n01440764/n01440764_10027.JPEG
	Read ssd(10ms)：从硬盘读图片到内存(path -> PIL.Image.Image)
	Transform(2ms)：PIL.Image.Image -> Torch.tensor

DatasetFolder.__getitem__(index=2)
	path, target = self.samples[2]  # path=n01440764/n01440764_10029.JPEG
	Read ssd(10ms)：从硬盘读图片到内存(path -> PIL.Image.Image)
	Transform(2ms)：PIL.Image.Image -> Torch.tensor

DatasetFolder.__getitem__(index=3)
	path, target = self.samples[3]  # path=n01440764/n01440764_10040.JPEG
	Read ssd(10ms)：从硬盘读图片到内存(path -> PIL.Image.Image)
	Transform(2ms)：PIL.Image.Image -> Torch.tensor

调用collate_fn函数(0.5ms*4)：
4个Torch.tensor[3x224x224] -> 1个Torch.tensor [4x3x224x224]

Batchsize=128时：read ssd时间1.5s，transform时间250ms，collate_fn时间70ms
```

从上图可以看出，下面三个变量比较重要，可以看出数据流向：

- index_queue[]：index_queue是一个数组，每个元素都是Queue()数据结构，不同进程间共享。index_queue[0]是worker进程0专属，其中每个元素是一个batchsize的图片索引（整形）。如图所示，index_queue[0]是黄色worker进程专属，里面存放了两个batchsize的索引，[0,1,2,3]和[8,9,10,11]。
- worker_result_queue：每个worker进程会重复以下工作（具体可见图片的右边文字部分）：从自己的index_queue取到一个batchsize的索引，以[0,1,2,3]为例，然后从硬盘读取0.jpg到内存，格式是PIL.Image.Image，然后再转成size是3x224x224的torch.Tensor，然后再读入1.jpg、2.jpg、3.jpg，此时就有了4个3x224x224的Tensor，再调用collate_fn把4个Tensor合并成一个4x3x224x22的大Tensor。再把这个Tensor扔到worker_result_queue里面去。
- data_queue：_worker_manager_loop线程会重复以下工作：从worker_result_queue中取出一个Tensor，将其在内存中固定地址，方便后续更快传输到gpu上，即进行pin_memory操作，再将被pin_memory的Tensor扔到data_queue中取，已供主进程读取。

数据加载主要涉及三个文件：

- github项目pytorch/examples：`imagenet/main_amp.py`
- torch库：`torch.utils.data.dataloader.py`
- torchvision库：`torchvision.datasets.folder.py`

main.py的核心代码是

```python
def main():
    # 生成ImageFolder类的对象：负责每张图片从硬盘的读取，及转成Tensor
    # 第一个参数：traindir代表训练数据集路径
    # 第二个参数：每张图片要做什么操作再转到Tensor，如resize、Flip等
    train_dataset = datasets.ImageFolder(
        traindir,
        transforms.Compose([
            transforms.RandomResizedCrop(224),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
            normalize,
        ]))
    # 生成DataLoader类的对象
    train_loader = torch.utils.data.DataLoader(
        train_dataset, batch_size=args.batch_size, shuffle=(train_sampler is None),
        num_workers=args.workers, pin_memory=True, sampler=train_sampler)
    # 训练数据集epochs次
	for epoch in range(args.start_epoch, args.epochs):
        train(train_loader, model, criterion, optimizer, epoch)
        prec1 = validate(val_loader, model, criterion)

# 每个epoch进行n次迭代
def train(train_loader, model, criterion, optimizer, epoch):
    for i, (input, target) in enumerate(train_loader):
        output = model(input)
        loss = criterion(output, target)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()        
```

dataloader.py核心代码

```python
class DataLoader(object):   
    def __iter__(self):
        return _DataLoaderIter(self)
    
class _DataLoaderIter(object):
    def __init__(self, loader):
        # 每个进程一个index队列
        self.index_queues = [multiprocessing.Queue() for _ in range(self.num_workers)]
        # 所以进程读取的batch都放在worker_result_queue中
        self.worker_result_queue = multiprocessing.SimpleQueue()
        # 开启num_workers个加载数据的进程
        self.workers = [multiprocessing.Process(
            target=_worker_loop,
            args=(..., self.index_queues[i],self.worker_result_queue,...))
                for i in range(self.num_workers)]
        # 开启1个用作pin_memory的线程
        self.worker_manager_thread = threading.Thread(
                    target=_worker_manager_loop,
                    args=(self.worker_result_queue, self.data_queue, ...))
        # worker_result_queue中的batch被pin_memory后放到data_queue中
        self.data_queue = queue.Queue()
        
    def _get_batch(self):
        return self.data_queue.get()
    
    def __next__(self):
        idx, batch = self._get_batch()
        return self._process_next_batch(batch)

# 加载数据的进程的函数入口
def _worker_loop(dataset, index_queue, data_queue, ...):
    while True:
        r = index_queue.get()
        idx, batch_indices = r
        samples = collate_fn([dataset[i] for i in batch_indices])
        data_queue.put((idx, samples))
        
# 用作pin_memory的线程的函数入口
def _worker_manager_loop(in_queue, out_queue, ...):
    while True:
        r = in_queue.get()
        idx, batch = r
        batch = pin_memory_batch(batch)
        out_queue.put((idx, batch))
```

folder.py核心代码

```python
class DatasetFolder(data.Dataset):
    def __getitem__(self, index):
        path, target = self.samples[index] # 得到某张图片的路径和label
        sample = self.loader(path)  # 从硬盘读图片到内存，存为PIL.Image.Image类
        sample = self.transform(sample)  # 将PIL.Image.Image转为Tensor，维度[3,224,224]
        return sample, target # 返回Tensor和label
```

## 3、内存到显存预加载

前面两个方法写的都是从**硬盘到内存**的数据读取加速，apex在此基础上，更是做到了数据从**内存到显存**的异步，主要通过cuda的stream实现。训练的每次迭代开始时，要将Tensor数据加载到显存里，一般会运行10ms左右，可以开启一个进程或者线程，将下一次迭代的数据提前加载到显存中，这样下次迭代运行时，数据就已经在显存了，此时再把下下次的数据提前加载到显存，如此下去。

具体可参考`nvidia`的giuhub项目`apex`中的训练[脚本](<https://github.com/NVIDIA/apex/blob/master/examples/imagenet/main_amp.py>)，其中的`data_prefetcher`类。

### 代码分析 - data_prefetcher类

在`__init__`函数中就调用了`preload`，实现第一batch从内存到显存的异步传输。主要是` with torch.cuda.stream(self.stream):`这个语句，被这个语句包含的块代码中，涉及的gpu操作，都会在self.stream这个自己管理的stream中执行。

在每次迭代中，调用next函数，第一句代码是等待self.stream中任务执行完，确保该轮迭代要用的数据已经传到gpu了。第二行和第三得到的input和target即是在gpu上准备好的数据。然后调用preload()继续下发任务：把下次迭代要用的数据传到gpu。下发完任务就马上返回。

```python
class data_prefetcher():
    def __init__(self, loader):
        self.loader = iter(loader)
        # 用户自己创建一个stream
        self.stream = torch.cuda.Stream()
        self.mean = torch.tensor([0.485 * 255, 0.456 * 255, 0.406 * 255]).cuda().view(1,3,1,1)
        self.std = torch.tensor([0.229 * 255, 0.224 * 255, 0.225 * 255]).cuda().view(1,3,1,1)
        # With Amp, it isn't necessary to manually convert data to half.
        # if args.fp16:
        #     self.mean = self.mean.half()
        #     self.std = self.std.half()
        self.preload()

    def preload(self):
        try:
         	# 获得内存中准备好的batch数据
            self.next_input, self.next_target = next(self.loader)
        except StopIteration:
            self.next_input = None
            self.next_target = None
            return
        # 在self.stream这个stream中传数据到gpu
        # 这里的.duda()或.float()只是给stream下发任务，并不等传到gpu再返回
        with torch.cuda.stream(self.stream):
            self.next_input = self.next_input.cuda(non_blocking=True)
            self.next_target = self.next_target.cuda(non_blocking=True)
            # With Amp, it isn't necessary to manually convert data to half.
            # if args.fp16:
            #     self.next_input = self.next_input.half()
            # else:
            self.next_input = self.next_input.float()
            self.next_input = self.next_input.sub_(self.mean).div_(self.std)
    
    # wait_stream的作用是等待self.stream这个stream执行完，即等待上次preload中的数据读完（也就是这次迭代的要用到的数据）
    def next(self):
        torch.cuda.current_stream().wait_stream(self.stream)
        input = self.next_input
        target = self.next_target
        self.preload()
        return input, target
```