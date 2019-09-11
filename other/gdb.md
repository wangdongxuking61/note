http://blog.jobbole.com/107759/
### 1、如何产生core file?
我们可以使用`ulimit`这条命令对core file文件的大小进行设定。在用这个命令的时候主要是为了产生core文件，
就是程序运行发行段错误时的文件。一般默认情况下，core file的大小被设置为了0，这样系统就不dump出core file了。
这时用如下命令进行设置
    
    ulimit -c unlimited
    
这样便把core file的大小设置为了无限大，同时也可以使用数字来替代unlimited，对core file的上限值做更精确的设定。

### 2、在编译时增加调试选项`-g`，才可以利用gdb来调试
    gcc hello.c -g -o hello
    //-g选项的作用是在可执行文件中加入源代码的信息，比如可执行文件中第几条机器指令对应源代码的第几行
    //但并不是把整个源文件嵌入到可执行文件中，所以在调试时必须保证gdb能找到源文件

### 3、gdb调试

    ./hello         #可执行程序测试
    gdb hello core  #gdb结合core文件调试
    (gdb) bt
    (gdb) Quit