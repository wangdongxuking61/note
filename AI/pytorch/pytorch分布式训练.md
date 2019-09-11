### 分布式训练简介

深度学习的神经网络在训练时，数据集都很大，如`CIFAR-10`有6万张图片，`ImageNet`甚至有约130万张图片。但是单个GPU的性能有限，需要使用多个GPU分布式训练。除了数据要切分到各个GPU卡，模型也要转化为分布式模型。PyTorch提供了`torch.nn.parallel.DistributedDataParallel`函数，能将单卡模型非常方便地转化为分布式模型。

但是PyTorch 0.4.1官方版本的**DistributedDataParallel**函数有两个重要的缺点：

- 每次迭代中，前向计算时会有额外开销
- 基于nccl聚合接口，反向传播完成后才进行一次性梯度通信，分布式训练时额外的通信开销很大

为例解决以上问题，nvidia发布了面向PyTorch的插件apex，引入新的DistributedDataParallel解决上述问题，将反向传播和梯度传输以pipeline的方式执行，通过计算和传输重叠隐藏计算开销。
同时，Pytorch也在1.0预览版提供了新的C10D库，基于相同的思想，通过更高效地异步执行方式，提高模型训练的效率。
两者都提供了梯度融合传输机制，将参数的梯度打包成一个固定大小的桶进行传输，但需要训练之前就设置桶的大小，PyTorch 1.0版通过`bucket_cap_mb`参数暴露给用户。

详情可参考：

PyTorch官方：https://pytorch.org/docs/stable/nn.html?highlight=distributeddataparallel

nvidia的apex插件：https://nvidia.github.io/apex/parallel.html

### python示例代码

```python
import os, time
import argparse
import torch
import torch.distributed as dist
import torch.nn.functional as F
import torch.optim as optim
from torchvision import models

parser = argparse.ArgumentParser(description='PyTorch ImageNet Training')
parser.add_argument("--local_rank", default=0, type=int)
parser.add_argument('--DDP-mode', choices=['torch', 'apex'])
args = parser.parse_args()

if __name__ == '__main__':
    # 定义模型
    torch.cuda.set_device(args.local_rank)  # 指定哪个GPU运行
    model = models.resnet50().cuda()

    # 将模型转化为分布式模型
    if 'WORLD_SIZE' in os.environ and int(os.environ['WORLD_SIZE']) > 1:  # 当WORLD_SIZE=1时, 只是单卡模式, 不必转化为分布式模型
        torch.distributed.init_process_group(backend='nccl', init_method='env://')
        if args.DDP_mode == 'torch':
            from torch.nn.parallel import DistributedDataParallel as DDP
            model = DDP(model, device_ids=[args.local_rank])
        elif args.DDP_mode == 'apex':
            from apex.parallel import DistributedDataParallel as DDP
            model = DDP(model)

    # 定义SGD优化器
    optimizer = optim.SGD(model.parameters(), lr=0.1)

    model.train()  # 将模型设置为train模式
    for i in range(100):
        # 1.定义一个batch的输入
        batch_size = 128
        data = torch.randn(batch_size, 3, 224, 224).cuda()  # 使用随机数据模拟图片输入, channel=3, height=224, width=224
        target = torch.tensor([1] * batch_size).cuda()  # 定义128张图片的类别id,这里全部设为类别1: [1, 1, 1, ..., 1]

        # 2.前向计算: 调用model的forward()方法
        torch.cuda.synchronize()  # 使用synchronize()同步等待所有cuda stream完成, 确保侧的时间准确
        fp_start = time.time()
        output = model(data)
        torch.cuda.synchronize()
        fp_time = time.time() - fp_start

        # 3.使用cross_entropy损失函数计算损失值
        loss = F.cross_entropy(output, target)

        # 4.反向计算梯度值
        torch.cuda.synchronize()
        bp_start = time.time()
        optimizer.zero_grad()  # 清空梯度历史值, 因为反向计算梯度的时候是累加的
        loss.backward()
        torch.cuda.synchronize()
        bp_time = time.time() - bp_start

        # 5.使用优化器更新模型参数
        optimizer.step()

        print("Iterstion %2d: time=%.3fs\tfp_time=%.3fs\tbp_time=%.3fs" % (i, fp_time + bp_time, fp_time, bp_time))
```

示例代码使用方式：

```bash
# 设定哪些GPU可用。可使用nvidia-smi命令查看GPU个数，GPU编号从0开始，如有8卡可用，编号是0~7
export CUDA_VISIBLE_DEVICES=0,1,2,3,4,5,6,7

# 单机单卡模式（原生PyTorch）
python main.py

# 单机多卡模式
# --nproc_per_node后面跟GPU使用个数
# --DDP-mode后跟使用哪种分布式，取值为torch、apex、pyfast
python -m torch.distributed.launch --nproc_per_node=2 main.py --DDP-mode torch
python -m torch.distributed.launch --nproc_per_node=2 main.py --DDP-mode apex

# 双机多卡模式
# 节点1
python -m torch.distributed.launch --nproc_per_node=1 --nnodes=2 --node_rank=0 --master_addr="192.168.1.1" --master_port=23333 main.py --DDP-mode torch
# 节点2
python -m torch.distributed.launch --nproc_per_node=1 --nnodes=2 --node_rank=1 --master_addr="192.168.1.1" --master_port=23333 main.py --DDP-mode torch
```

### 两种DistributedDataParallel性能对比

系统环境和脚本配置

- GPU型号：V100-PCIE-32G
- TyTorch：0.4.1
- torchvision：0.2.1
- Model：ResNet50
- 单张卡的Batch size=128
- 输入图片：尺寸是3x224x224。这里使用torch.randn()函数随机生成，仅用于方便示例
- 迭代次数：100

| 单轮迭代时间（前向+反向）/ms |   PyTorch    | nvidia apex  |
| :--------------------------: | :----------: | :----------: |
|           单机1卡            | 371(119+252) |      无      |
|           单机2卡            | 433(134+291) | 423(131+291) |
|           单机4卡            | 438(138+301) | 430(131+299) |
|           单机8卡            | 450(136+313) | 437(131+306) |