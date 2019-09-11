### 半精度训练简介

神经网络在训练或推理时，一般使用4 Bytes表示一个浮点数，即`单精度`（简称fp32）。如果使用2 Bytes表示的话，即`半精度`（简称fp16），可省近一半的存储，并且在分布式训练的时候，传输数据量也少了一半。

目前支持fp16运算的显卡只有P100和V100：

- P100计算fp16的速度比fp32快约10%
- V100是Nvidia最新一代的显卡，具有专门针对fp16计算的硬件`Tensor Core`，能大大加快fp16运算的速度，V100计算fp16的速度是P100的12倍

如果您手里有V100显卡，强烈建议您使用`半精度训练`或`混合精度训练`（绝大多数参数使用半精度保存，少部分参数使用单精度保存），能大大加速训练过程，且训练的准确率和收敛性都不会变化。`混合精度训练`可参考baidu和nvidia的[论文](https://arxiv.org/abs/1710.03740)和nvidia开源的pytorch的插件[apex](https://github.com/NVIDIA/apex)。

这里使用ResNet50模型单卡训练为例，对PyTorch官方的半精度接口做简略说明。

### python示例代码

```python
import time
import argparse
import torch
import torch.nn.functional as F
import torch.optim as optim
from torchvision import models

parser = argparse.ArgumentParser(description='PyTorch half precision training demo')
parser.add_argument('--fp16', action='store_true', help='Run model fp16 mode.')
args = parser.parse_args()

if __name__ == '__main__':
    # 定义模型和SGD优化器
    model = models.resnet50().cuda()
    if args.fp16:
        model.half()
    optimizer = optim.SGD(model.parameters(), lr=0.1)

    model.train()  # 将模型设置为train模式
    for i in range(100):
        # 1.定义一个batch的输入
        batch_size = 128
        data = torch.randn(batch_size, 3, 224, 224).cuda()  # 使用随机数据模拟图片输入, channel=3, height=224, width=224
        target = torch.tensor([1] * batch_size).cuda()  # 定义128张图片的类别id,这里全部设为类别1: [1, 1, 1, ..., 1]
        if args.fp16:
            data = data.half()

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
# 默认使用单精度训练（fp32）
python main.py

#  带--fp16参数，则使用半精度训练（fp16）
python main.py --fp16

# 指定GPU运行，可使用nvidia-smi命令查看GPU个数。GPU编号从0开始，如有8卡可用，编号是0~7
CUDA_VISIBLE_DEVICES=0 python main.py
```

通过观察python代码可知，如果执行脚本时使用` -fp16`参数，代码会多执行了两条语句：

- `model.half()`：使用torchvision生成模型会后，只需执行`model.half()`就能把模型转化为半精度的模型
- `input.half()`：模型的数据输入也要相应的变成半精度数据

这就是PyTorch对半精度训练的支持，只需要将模型和输入数据转化为fp16（执行half()函数）就行。

### fp32和fp16性能对比

系统环境和脚本配置

- GPU型号：V100-PCIE-32G
- TyTorch：0.4.1
- torchvision：0.2.1
- Model：ResNet50
- Batch size=128
- 输入图片：尺寸是3x224x224。这里使用torch.randn()函数随机生成，仅用于方便示例
- 迭代次数：100

|                    | 单轮迭代时间（前向+反向）/ms | 前向时间/ms | 反向时间/ms | 占用显存/MB |
| :----------------: | :--------------------------: | :---------: | :---------: | :---------: |
| 单精度训练（fp32） |             406              |     129     |     277     |    13336    |
| 半精度训练（fp16） |             345              |     107     |     238     |    7262     |