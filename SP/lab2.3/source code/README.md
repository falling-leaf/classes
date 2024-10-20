这里是SP2.3的源代码，其文件夹及文件结构如下：
- for_32: gcc32位编译方法对应的实验文件
- for_64: gcc正常编译方法对应的实验文件

在每个文件夹中有如下文件：
- badfile: stack读取的文件
- exploit: exploit.c的可执行文件
- exploit.c: 用于生成badfile
- stack: stack.c的可执行文件
- stack.c: 用于进行缓冲区溢出的主要文件

这些文件是在`ubuntu22.04`系统环境下进行编译及运行的，如需检测需要注意如下几点：

- 首先要输入下面代码以解除内存地址的随机化：
```
$ su root
  Password: (enter root password)
# sysctl -w kernel.randomize_va_space=0
```
- 由于不同系统中对栈地址位置会有所不同，因此需要对`stack`进行gdb调试，找到对应的地址，并在`exploit.c`做出相应的修改，之后才能够正常运行文件。

note:在32位中偏移量为24，在64位中偏移量为20.

具体操作步骤见实验报告。