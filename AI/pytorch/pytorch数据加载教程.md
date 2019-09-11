# PyTorch数据加载教程 

### 1 先使用torchvision.datasets中的接口创建Dataset类

举几个常见的Dataset类：

- torchvision.datasets.CIFAR10
  - cifar10数据集，共十个类别：飞机（airplane），汽车（automobile），鸟（bird），猫（cat），鹿（deer），狗（dog），青蛙（frog），马（horse），船（ship），卡车（truck）
  - 5万张训练图片，1万张测试图片
  - 28x28的彩色RGB图片
- torchvision.datasets.MNIST
  - mnist数据集，0到9的数字手写体
  - 6万张训练图片，1万张测试图片
  - 28x28的灰度图
- torchvision.datasets.ImageFolder
  - 一种通用的图片数据集存储格式，可用于读取ImageNet这样的数据集
  - 路径形如：
    - root/dog/xxx.png
    - root/dog/xxy.png
    - root/dog/xxz.png
    - root/cat/123.png
    - root/cat/nsdf3.png
    - root/cat/asd932_.png

也可实现自己的Dataset类，但要继承torch.utils.data.Dataset这个父类，并重写这三个方法

- def __getitem__(self, index):
- def __len__(self):
- def __repr__(self): 

torch.utils.data.Dataset的源码如下（torch/utils/data/dataset.py文件）：

```Python
class Dataset(object):
    """An abstract class representing a Dataset.

    All other datasets should subclass it. All subclasses should override
    ``__len__``, that provides the size of the dataset, and ``__getitem__``,
    supporting integer indexing in range from 0 to len(self) exclusive.
    """

    def __getitem__(self, index):
        raise NotImplementedError

    def __len__(self):
        raise NotImplementedError

    def __add__(self, other):
        return ConcatDataset([self, other])
```

### 2 创建Sampler类

数据集准备好了，里面图片的数量和顺序就固定了。但训练的时候并不总是从第一张到最后一张读取。有时候还要打乱顺序，甚至在分布式训练的时候还要将数据集分块。此时需要一个`确定图片使用顺序`的类，是顺序使用、还是随机使用。PyTorch提供了一个叫做torch.utils.data.Sampler的类，用于确定”采样”的方式，即确定图片的使用顺序。

#### 2.1 常用的Sampler有三种：SequentialSampler、RandomSampler、DistributedSampler

举个例子，如果数据集里面有5张图片：

- SequentialSampler的__iter__方法每次的返回值分别是0、1、2、3、4。
- RandomSampler的__iter__方法每次的返回值分别是0、1、2、3、4的乱序，如1、2、0、3、4。
- DistributedSampler比较特殊，是RandomSampler的分布式版本，在分布式训练的时候才使用。例如有两个GPU一起训练时，PyTorch会为每个GPU生成一个DistributedSampler，并补齐图片到6张（确定能被GPU数量整除），生成随机数列，有可能是5、1、2、0、4、3。为所有的DistributedSampler设置相同的随机种子，即可得到相同的随机数列。GPU 0要数列前一半，GPU 1要后一半，即GPU 0的DistributedSampler的__iter__方法每次的返回值分别是5、1、2，GPU 1的是0、4、3。

#### 2.2 在Sampler的基础上有BatchSampler的概念，理解也很简单，就是每次返回batch_size大小的图片序号

BatchSampler的初始化参数有三个：

|            | 类型                     | 说明                                                       |
| ---------- | ------------------------ | ---------------------------------------------------------- |
| sampler    | torch.utils.data.Sampler | 确定图片使用顺序的Sampler类                                |
| batch_size | int                      | 训练时mini-batch的大小                                     |
| drop_last  | bool                     | 如果最后一个batch不足batch_size大小，drop_last决定是否丢弃 |

接着举个例子，接着上面2.1的例子

- 如果这样初始化：BatchSampler(SequentialSampler, batch_size=2, drop_last=False)。则BatchSampler的__iter__方法每次返回值是[0,1]、[2,3]、[4]。
- 如果使用RandomSampler用作初始化参数：BatchSampler(RandomSampler, batch_size=2, drop_last=False)。则BatchSampler的__iter__方法每次返回值是随机的，有可能的一种情况是[2,1]、[0,3]、[4]。

### 3 创建torch.utils.data.DataLoader类用于训练

示例如下：

```python
train_loader = torch.utils.data.DataLoader(dataset=train_dataset, 
                                           batch_size=args.batch_size, 
                                           num_workers=args.workers, 
                                           shuffle=(train_sampler is None),
                                           pin_memory=True, 
                                           sampler=train_sampler)
# 一个epoch的训练过程
for i, (input, target) in enumerate(train_loader):
    # 前向计算
    # 反向传播
    # 梯度更新
```

`DataLoader`参数说明：

- train_dataset：已经创建好的torch.utils.data.dataset类
- batch_size：mini-batch size的大小
- num_workers：读取图片的子进程数量，使用多个进程一起工作，读图速度更快。**训练过程中，总的num_workers数量要小于计算机的`逻辑核心数
- shuffle：是否随机采样
- pin_memory： 如果是true，将tensor拷贝到GPU上之前要在内存中做pin memory操作，让拷贝速度更快
- sampler：已经创建好的Sampler类。sampler的值非None和shuffle=True会冲突。因此shuffle=True时不能设sampler；设sampler时，不设shuffle，或shuffle=False。