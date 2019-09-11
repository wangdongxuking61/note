## 1 什么是混合精度训练？

关于混合精度训练，百度和nvidia合作的这篇论文最权威：[MIXED PRECISION TRAINING](https://arxiv.org/abs/1710.03740)。

### 1.1 使用fp16（半精度）的好处

众所周知，搞深度学习需要高端硬件的支持，那是因为模型中的参数（weight）、梯度（gradient）、feature map的数据量很大，不光会占用大量的内存或显存，而且分布式训练时的数据传输量（传梯度）也很大。

即使是到2015年，深度学习模型中的数据还是用32位浮点数表示。因此有人就想使用16位浮点数替代之，好处如下：

- 省存储：参数、梯度、feature map均表示为fp16，省一半存储（显存或内存），因此可使用更大的batch-size
- 省传输：分布式训练时（单机多卡，或多机多卡），梯度的传输量减少一半
- 省计算：目前支持fp16运算的显卡有P100和V100。在P100上，fp16运算比fp32运算快一点，但不明显，因为没有专门硬件做fp16运算，要使用FP32 core（计算fp32的硬件）去模拟。但是V100有专门的硬件（tensor core）计算fp16。至于cpu，肯定是支持fp16计算的，但需要软件库的支持，比如说python的numpy，具体fp16比fp32在cpu上快多少没测过，因为没人在cpu上训练。

### 1.2 既然有了fp16，为什么还要混合精度？

**混合精度训练的定义：训练模型时，大多数数据用fp16表示，少数用fp32表示。**

既然使用fp16有这么多好处，为什么训练模型时不把所有数据都用fp16表示呢？ 那是因为使用fp16有以下两个限制：

- 相比于fp32，fp16的表示精度比较低
- 某些梯度数据用fp16表示不了 ，如小于2^-24的数字

## 2 混合精度训练代码

NVIDIA官方SDK文档已有混合精度训练教程，[已支持各个主流框均](https://docs.nvidia.com/deeplearning/sdk/mixed-precision-training/index.html)。

### 2.1 PyTorch实现混合精度训练

Nvidia在pytorch上实现了一个插件：[apex](https://github.com/nvidia/apex)（A PyTorch Extension: Tools for easy mixed precision and distributed training in Pytorch），实现了混合精度训练。

在此基础上，我们将apex中混合精度训练的核心功能FP16_Optimizer集成到我们pyfast代码中。

### 2.2 pyfast.apex.fp16_utils.FP16_Optimizer接口说明

|                    | 类型                  | 默认值 | 说明                                                         |
| ------------------ | --------------------- | ------ | ------------------------------------------------------------ |
| optimizer          | torch.optim.optimizer | 无     | 想要被转化为混合精度训练模式的优化器                         |
| static_loss_scale  | float                 | 1.0    | loss的缩放系数                                               |
| dynamic_loss_scale | bool                  | False  | 是否使用loss自动缩放功能，如果为True，则static_loss_scale无效 |

用户只需要将原来的optimizer替换成FP16_Optimizer，其它代码基本不用修改，即可使用混合精度训练功能。

### 2.3 代码示例

在ImageNet真实数据集上使用apex的fp16_optimizer实现混合精度训练。用户可修改脚本中的数据集为自己的即可实现混合精度训练。

[Python示例代码链接](https://github.com/NVIDIA/apex/blob/master/examples/imagenet/main_amp.py)

使用说明：

```bash
# 单卡训练
python main_amp.py /path/to/imagenet/folder

# 单卡训练（指定GPU卡）
CUDA_VISIBLE_DEVICES=1 python main_amp.py /path/to/imagenet/folder

# 单卡训练：loss缩放系数为128
python main_amp.py --static-loss-scale 128.0 /path/to/imagenet/folder

# 多卡训练：loss缩放系数为128
python -m torch.distributed.launch --nproc_per_node=2 main_amp.py --loss-scale 128.0 /path/to/imagenet/folder

# 多卡训练：使用FP16_Optimizer，使用动态的loss缩放系数
python -m torch.distributed.launch --nproc_per_node=2 main_amp.py --dynamic-loss-scale /path/to/imagenet/folder
```