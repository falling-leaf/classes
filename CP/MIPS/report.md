---

title: Title
theme: sky
highlightTheme: tomorrow-night-bright
revealOptions:
width: 1520
height: 950
margin: 0.04
transition: 'convex'
slideNumber: true

---


### M I P S - p r o g r a m
##### 回 滚 膜 队
##### 2 0 2 3 . 1 1

---

#### P a r t 1 综 述

----

##### P a r t 1 . 1  编 程 语 言 及 实 现 功 能

<font size = "6" face = "SimHei">

本程序使用<font color = "cyan">C++</font>语言进行编程，实现的功能如下：

1. 对基本的R、I、J类型指令可作如下操作：
   - 汇编
   - 反汇编
   - 执行

2. 对输入的整数、浮点数可作如下操作：
   - 直接输出该数的二进制表示
   - 如果存在运算符，输出运算的结果

3. <font color = "orange">（bonus）搭建前端界面</font>
   - 利用Qt搭建方便美观的前端界面，方便用户进行使用
</font>

----

##### P a r t 1 . 2  多 文 件 构 成

<font size = "5" face = "SimHei">
本程序由如下多个文件构成，并对所有头文件进行<font color = "orange">文件保护操作</font>：

- 主程序：main.cpp
  - 分程序1：汇编
  - 分程序2：反汇编
  - 分程序3：整数、浮点数
  - 分程序4：执行
  - 辅助文件：Qt部分函数声明及实现

</font>

``` cpp[1|2-3|4-5|6-7|8-9|10-12]
  #include "main.cpp"
  #include "assembler.h"
  #include "assembler.cpp"
  #include "antiassembler.h"
  #include "antiassembler.cpp"
  #include "converter.h"
  #include "converter.cpp"
  #include "executor.h"
  #include "executor.cpp"
  #include "widget.ui"
  #include "widget.h"
  #include "widget.cpp"
```

---

#### P a r t 2  汇 编 器

----

##### P a r t 2 . 1  基 本 数 据 结 构
<font size = "6" face = "SimHei">

使用类<font color = "orange">"codeline_for_ass"</font>读取、处理每一行代码并输出结果.

类内变量部分如下：
- type：指令类型
- func：保存指令的功能码
- label_address：保存标签
- currentAddress：当前指令地址
- instruction：当前行内容
- output：转换后的机器码

``` cpp [4|5|6|8|9|10]
class codeline_for_ass
{
private:
    char type;
    string func;
    map <string, int> label_address;
public:
    int currentAddress;
    string instruction;
    string output;
};

```
</font>
----

##### P a r t 2 . 2  执 行 流 程
<font size = "6" face = "SimHei">

1. 第一次扫描，依次读取每行代码完成标签声明记录
2. 第二次扫描，依次处理每一行代码
   - 单行处理判断类型并进行寄存器和地址等处理
   - 将处理之后的机器码保存在字符串中
3. 输出结果

``` cpp [3-20|21-35|36]
void assembler_exe()
{
    codeline_for_ass line1;
    QFile fileW("./output.txt");
    fileW.open(QIODevice::WriteOnly);
    QTextStream out(&fileW);
    string result="";
    QFile fileR1("./input.txt");
    fileR1.open(QIODevice::ReadOnly);
    QTextStream in1(&fileR1);
    int PC = 0;
    while (!in1.atEnd()) {
        QString qstr;
        qstr = in1.readLine();
        qstr =qstr.trimmed();
        if(qstr.isEmpty()) continue;
        line1.instruction = qstr.toStdString();

        line1.process_label_address(PC);
    }
    QFile fileR2("./input.txt");
    fileR2.open(QIODevice::ReadOnly);
    QTextStream in2(&fileR2);
    PC = 0;
    while (!in2.atEnd()) {
        QString qstr;
        qstr = in2.readLine();
        qstr =qstr.trimmed();
        if(qstr.isEmpty()) continue;
        line1.instruction = qstr.toStdString();
        line1.currentAddress = PC; PC += 4;
        line1.assemble();
        result = result + line1.output + '\n';
        line1.output = "";
    }
    out << QString::fromStdString(result);
```

</font>

---

#### P a r t 3  反 汇 编 器

----

##### P a r t 3 . 1  基 本 数 据 结 构
<font size = "6" face = "SimHei">

- 对每一行代码用类<font color = "orange">"codeline_for_antia"</font>来处理
- 类内变量部分如下所示：
  - input、output分别存储获得的机器码与处理后的结果
  - 所有bool变量表示该行代码代表何种类型
``` cpp[12-13|4-11]
class codeline_for_antia
{
private:
    bool is_RType = false;
    bool is_IType = false;
    bool is_JType = false;
    bool is_SpecR = false;
    bool is_jr = false;
    bool is_lui = false;
    bool is_lwsw = false;
    bool is_beq = false;
    string input;
    string output;
}
```

</font>

----

##### P a r t 3 . 2  执 行 流 程
<font size = "6" face = "SimHei">

1. 读取一行机器码
2. 对机器码所属类型（*R/I/J*）进行判断
3. 分别对各个类型的指令进行汇编语言改写
4. 输出该行机器码对应的汇编语言

``` cpp [3-10,14-19|20|21-26|27,29]
void antiassembler()
{
    codeline_for_antia codeline;
    initialize_register();
    QFile fileR("./input.txt");
    fileR.open(QIODevice::ReadOnly);
    QTextStream in(&fileR);
    QFile fileW("./output.txt");
    fileW.open(QIODevice::WriteOnly);
    QTextStream out(&fileW);
    string result = "";
    while (!in.atEnd())
    {
        codeline.clear_string();
        QString qstr;
        qstr = in.readLine();
        qstr = qstr.trimmed();
        if(qstr.isEmpty()) continue;
        codeline.input = qstr.toStdString();
        codeline.decode_operation();
        if (codeline.read_Type() == 'R')
            //codes...
        else if (codeline.read_Type() == 'I')
            //codes...
        else if (codeline.read_Type() == 'J')
            //codes...
        result = result + codeline.output + '\n';
    }
    out << QString::fromStdString(result);
}
```
</font>
---

#### P a r t 4  整 数 、浮 点 数 操 作

----

##### P a r t 4 . 1  基 本 数 据 结 构

<font size = "6" face = "SimHei">

- 整数、浮点数操作中不涉及大段内容的存储，因此将所有函数封装在类<font color = "orange">"NumberConverter"</font>中。
- 经过数据类型(*string/int/float*)之间的处理与转换，converter可以实现二进制补码与十进制的转化，并且具备计算功能。
</font>

----

##### P a r t 4 . 2  执 行 流 程

<font size = "5" face = "SimHei">

1. 逐行读取输入文件的内容，判断是否为二进制补码。
2. 如果是32位二进制数，将二进制补码转换为十进制整数和浮点数并输出结果。
3. 如果不是二进制补码格式，则提取数值并根据可能给出的运算符进行计算。
4. 重复2~4步骤，遍历全部指令。

``` cpp [2-4,8-11|12-19|26-36|20-23,37-38,41-42]
void converter() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt"); 
    //codes for input...
    string line;
    while(!in.atEnd())
    {
        QString qstr;
        qstr = in.readLine();
        qstr =qstr.trimmed();
        line = qstr.toStdString();
        if (line.length() == 32) {
            int intResult = NumberConverter::ComplementToInt(line);
            float floatResult = NumberConverter::ComplementToFloat(line);
            if(intResult == -1 || floatResult == -1)
            {
                out << "Input Error\n";
                continue;
            }
            string tmp_int,tmp_float;
            tmp_int = to_string (intResult);
            tmp_float = to_string (floatResult);
            //codes...
        }
        else {
            stringstream ss(line);
            double num1, num2;
            char op;
            if(ss >> num1 >> op >> num2) {
                double ans;
                int decimal_place1 , decimal_place2, place;
                decimal_place1 = get_decimal(num1);
                decimal_place2 = get_decimal(num2);
                place = max (decimal_place1,decimal_place2);
                //codes...
                }
                out << QString::number(ans,'f',place);
                out << "\n";
            }
            else{
                string result = NumberConverter::detectAndConvert(line)+ "\n";
                out << QString::fromStdString(result);
            }
        }
    }
}
```
</font>
---

#### P a r t 5  执 行 器

----

##### P a r t 5 . 1  基 本 数 据 结 构
<font size = "6" face = "SimHei">

- 执行器中分别需要如下结构体来执行程序并存储结果：
  - 寄存器
  - 指令
  - 内存

``` cpp [1-6|7-12|13-18]
typedef struct
{
    int ind;
    int val;
    string name;
}Register;
typedef struct
{
    int addr;
    string ins;
    string label;
}Program;
typedef struct
{
    int addr;
    int val;
    string addr_16;
}Memory;
```
</font>
----

##### P a r t 5 . 2  执 行 流 程
<font size = "6" face = "SimHei">

1. 初始化内存、寄存器并赋初值，获取输入的汇编语言
2. 第一次遍历，获取所有标签及其位置
3. 第二次遍历，用PC作为模拟指针进行执行器操作
4. 输出寄存器及内存的值


``` cpp [11-12|6|1-5,7,13|14]
void add(string rd, string rs, string rt);
void sub(string rd, string rs, string rt);
void addi(string rt, string rs, int immNum);
void lw(string rt, string rs, int immNum);
//and other aseembly functions...
string GetLabel(string str);
int checkLabel(string str);
void executor()
//more information is in every function
{
    initialize();
    Input();
    Work();
    Output();
}
```
</font>

---

#### P a r t 6  前 端 设 计

----

##### P a r t 6 . 1  设 计 简 介
<font size = "4" face = "SimHei">

- Qt是一个方便的前端开发工具，可以令用户简单地实现前端界面的设计与操作。
- 通过设计，最终界面展示效果如下：
![1700720580353](image/report/1700720580353.png)

</font>

----

##### P a r t 6 . 2  前 端 与 源 代 码 链 接
<font size = "6" face = "SimHei">

- 由于Qt本身是由C++语言构成的，因此也提供了C++语言程序实现相应的端口，通过如下流程可实现二者的链接：
1. Qt导入QString形式的输入
2. QString转为string数据类型并流入源代码处理
3. 处理后的string类型的结果转化为QString
4. Qt导出QString形式的结果
```cpp [3-10|11|12-16]
void Widget::on_pushButton_2_clicked()
{
    QString input_data = ui->textEdit_1->toPlainText();
    QString output_data;
    string tempstr = "";
    QFile fileW("./input.txt");
    fileW.open(QIODevice::WriteOnly);
    QTextStream out(&fileW);
    out << input_data;
    fileW.close();
    assembler_exe();
    QFile fileR("./output.txt");
    fileR.open(QIODevice::ReadOnly);
    output_data = fileR.readAll();
    ui->textEdit_2->setPlainText(output_data);
    fileR.close();
}
```

----

##### 运行测试：汇编
<font size = "6" face = "SimHei">

![1701335635518](image/report/1701335635518.png)
</font>



----

##### 运行测试：反汇编
<font size = "6" face = "SimHei">

![1701335627648](image/report/1701335627648.png)
</font>

----

##### 运行测试：整数、浮点数
<font size = "6" face = "SimHei">

![1701335545630](image/report/1701335545630.png)
</font>

----

##### 运行测试：执行器
<font size = "6" face = "SimHei">

![1701335619214](image/report/1701335619214.png)
</font>

---

#### P a r t 7  成 员 分 工
<font size = "6" face = "SimHei">

- 汇编器设计：吕梦月
- 反汇编器设计：王昊元
- 整数、浮点数器设计：余卓耘
- 执行器设计：任飞扬
- 前端设计：任飞扬
- PPT设计：王昊元

</font>

---

### Thanks！

----
