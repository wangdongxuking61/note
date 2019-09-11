### mobilenet-v1

inp=32，outp=32

- 先dw：32个3x3，横向

- 再pw：32x32x1x1，全连

### mobilent-v2

#### 个人总结

- 使用了resnet的shortcut
- linear bottleneck：bottleneck输出不用relu
- Inverted：和resnet一样的话，降维后信息损失严重，需要升维再降维

#### 和resnet、mobilenet-v1结构对比

![img](https://pic1.zhimg.com/80/v2-25b6c783dbb5412119200696f02f3018_hd.jpg)



- 首先是`1×1 conv2d`变换通道，后接ReLU6激活(ReLU6即最高输出为6，超过了会clip下来)
- 中间是深度卷积,后接ReLU
- 最后的`1×1 conv2d`后面不接ReLU了，而是论文提出的linear bottleneck

#### linear bottleneck

![å¨è¿éæå¥å¾çæè¿°](https://img-blog.csdnimg.cn/2018122012502685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE5NzQ2Mzk=,size_16,color_FFFFFF,t_70)

- 图(a)：普通模型架构使用标准卷积将空间和通道信息一起映射到下一层，参数和计算量会比较大
- 图(b)，MobileNetv1中将标准卷积拆分为深度卷积和逐点卷积，深度卷积负责逐通道的过滤空间信息，逐点卷积负责映射通道。将空间和通道分开了
- 图©和图(d)是MobileNetv2的结构(d是c的下一个连接状态)，同样是将标准卷积拆分为深度卷积和逐点卷积，在逐点卷积后使用了接1×1 1×11×1卷积，该卷积使用线性变换，总称为一层低维linear bottleneck，其作用是将输入映射回低维空间

由上面的分析，直觉上我们认为linear bottleneck中包含了所有的必要信息，对于Expansion layer(即linear到深度卷积部分)仅是伴随张量非线性变换的部分实现细节，我们可将shortcuts放在linear bottleneck之间连接。示意图如下：

![å¨è¿éæå¥å¾çæè¿°](https://img-blog.csdnimg.cn/20181220125033921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE5NzQ2Mzk=,size_16,color_FFFFFF,t_70)

#### Inverted residuals

通常的residuals block是先经过一个1x1的Conv layer，把feature map的通道数“压”下来，再经过3x3 Conv layer，最后经过一个1x1 的Conv layer，将feature map 通道数再“扩张”回去。即先“压缩”，最后“扩张”回去。 

![img](https://img-blog.csdn.net/20180916091244547?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dieXk0MjI5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而 inverted residuals就是 先“扩张”，最后“压缩”。为什么这么做呢？请往下看。

在上图中，我们可以看到：

- ResNet是：压缩”→“卷积提特征”→“扩张”，MobileNetV2则是Inverted residuals,即：“扩张”→“卷积提特征”→ “压缩”：经过1x1的卷积可以扩大通道数，**提升其抽取特征的能力**。

- 最后不采用Relu，而使用Linear代替：Relu对于负的输入，输出全为零；而本来特征就已经被“压缩”，再经过Relu的话，又要“损失”一部分特征，因此这里不采用Relu。

总结：在通道数较少的层后，应该用线性激活代替ReLU。MobileNet V2的Linear bottleneck Inverted residual block中，降维后的1X1卷积层后接的是一个线性激活，其他情况用的是ReLU。

### shufflenet-v1

![img](https://pic2.zhimg.com/80/v2-4cb10ae8036f15cdc45280473c5f2cd1_hd.jpg)

![img](https://pic3.zhimg.com/80/v2-fdf77861cd24ab65be0c81934e7e3af2_hd.jpg)