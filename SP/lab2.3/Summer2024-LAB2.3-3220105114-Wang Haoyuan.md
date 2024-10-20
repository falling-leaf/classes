<div align='center'>
	<font face = "Arial" size="5">Security Programming</font>
</div>

<div align='center'>
	<font face = "Arial" size="5">Lab 2.3</font>
</div>

<div align='center'>
	<font face = "Arial" size="5">Wang Haoyuan</font>
</div>

<p style="page-break-before:always;"></p>

# Lab2.3: Buffer Overflow Vulnerability

## Step 1: 配置相关ubuntu环境

由于实验推荐ubuntu版本为12.04，因此在ubuntu官网中下载12.04镜像，并安装虚拟机：

![1718071614267](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718071614267.png)

启动后页面如下：

![1718071751623](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718071751623.png)

## Step 2: 禁用相关保护功能：

### 1. 禁用地址随机化：

![1718072013468](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718072013468.png)

### 2. 取消ExecShield Protection:

尝试取消后发现并没有这个保护：

![1718072113684](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718072113684.png)

查阅博客后发现Ubuntu系统中应当没有这个屏蔽，那么继续实验。

## Step 3: 创建易受攻击的程序：

按照实验步骤，创建 `stack.c`:

![1718072546046](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718072546046.png)

通过root账户来编译它（32位），并将可执行文件更改为4755：

![1718089584838](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718089584838.png)

![1718115002025](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718115002025.png)

## Step 4: 通过gdb解析相关地址，并对exploit.c进行补全：

通过`gdb stack`, `disass bof`来具体解析bof内的汇编语言：

![1718089635142](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718089635142.png)

我们发现对于esp和edx是其中的关键节点，那么我们需要对它们所在的行设置断点，并且对它们的实际地址进行记录：

![1718118659185](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718118659185.png)

那么$0xffffcce8 - 0xffffccd4 + 0x4= 0x18 = 0d24$，因此得出buffer的偏移量为24。

接下来对exploit.c进行补全：


![1718118814774](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1718118814774.png)

## Step 5: 尝试编译并运行：

编译exploit.c文件，并执行exploit与stack，可以发现跳转到了对应的命令行。输入`whoami`后可以查看当前的用户名称：

![1719063544560](image/Summer2024-LAB2.3-3220105114-WangHaoyuan/1719063544560.png)

## 小结：

本实验主要通过模拟缓冲区溢出的情况，对系统内部进行攻击。由于该实验相对复杂，因此进行了多次不同的尝试才成功将程序运行起来。其中包括分别通过`ubuntu12.04`与`ubuntu22.04`及32位编译与64位编译四种方式进行尝试。其中由于`ubuntu12.04`版本过低似乎无法安装编译32位的gcc库，因此中途改换成了`ubuntu22.04`系统。最终发现`ubuntu22.04`也可以完成本实验。
