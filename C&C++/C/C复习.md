# C语言复习

本篇文章是对C语言复习的笔记，不适合新手入门。适合先学习C++，但没用系统学习过C语言的朋友，也适合想要复习和回顾C语言的朋友。

C语言是一门十分重要的语言，是计算机科学体系中十分关键的部分。

这里就跳过编译器与环境的教程了，可以自行百度。

C语言写出的.c源码，需要经过编译器的处理，形成.obj的目标文件，之后链接器将多个.obj文件和库文件进行链接，形成.exe可执行文件。

## 基础

这里写一个第一个C程序：

```C
int main()
{
    printf("Hello word!");
    return 0;
}
```

### main函数

C的主程序是从main函数开始的，main函数是程序的入口，也被较为主函数。

注意，一个项目中可以有很多个.c文件，但是只能有一个main函数。

约定主函数main的返回值是int类型，而约定当程序运行成功时返回0，运行失败时返回1。

当然main的返回值也可以是void：
```c
void main(){
    //...
}
```

这是一种旧式的写法，如何在读书或者读代码的时候遇到了这样的写法，说明是有时间沉淀，年份久远的。

main函数也是可以有参数的，类似下面这样：
```c
int main(int argc, char* argv[],char* envp[]){

    return 0;
}
```
参数有特殊的意义，其中 argc 和 argv 作为 main 函数的参数，主要用于接收命令行参数。

`argc`（argument count，参数计数）
表示命令行参数的个数，包括程序本身的名称。
它的值至少为 1（当不传递其他参数时，仅包含程序名）。

`argv`（argument vector，参数向量）
是一个字符串数组，每个元素都是一个命令行参数。
argv[0]：程序的名称（通常是可执行文件的路径）。
argv[1] 及后续元素：用户输入的命令行参数。

示例：

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("参数个数: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("参数 %d: %s\n", i, argv[i]);
    }
    return 0;
}
```

编译为program后在终端运行：
```
./program hello world 123
```

可以输出结果：
```
参数个数: 4
参数 0: ./program
参数 1: hello
参数 2: world
参数 3: 123
```

### printf和库函数

```c
printf("hello C\n");
```

printf是库函数，实现了在屏幕上打印信息。可以将两个引号中间的内容显示在屏幕上。

同时可以显示其他信息：

```c
int n = 100;
printf("%d\n",n); //打印整数型
printf("%d\n",'q'); //打印字符
printf("%d\n",3.14); //打印双精度浮点型
```

这里的%d是占位符，在后面还会提到。

导入标准输入输出库：
```c
include <stdio.h>
```

关于什么是库函数呢，写一段时间的代码自然会明白，C语言的库函数比较多，详细可以参考链接：https://cplusplus.com/reference/clibrary/

### 关键字

C语言中有一批关键的保留字，这些字有关键的意义，是留给C语言使用的，程序员在创建变量名的时候不可以使用：

```
auto  break   case  char  const   continue  default  do   double else  enum   ex
 float  for   goto  if   int   long  register    return   short  signed  sizeof  
struct  switch  typedef union  unsigned   void  volatile  while
```

注：在C99标准中加⼊了下面这些关键字：

```
inline restrict _Bool _Comploex _Imaginary
```

关于C语言全部关键字的介绍可以去下面这个链接：https://zh.cppreference.com/w/c/keyword

### 字符串和ASCLL编码：

在键盘上可以敲出各种字符，如：a，q，@，#等，这些符号都被称为字符，C语⾔中字符是⽤单引号括起来的，如：'a'，'b'，'@'。

我们知道在计算机中所有的数据都是以⼆进制的形式存储的，那这些字符在内存中分别以什么样的⼆进制存储的呢？如果我们每个⼈⾃⼰给这些字符中的每个字符编⼀个⼆进制序列，这个叫做编码，为了⽅便⼤家相互通信，不造成混乱，后来美国国家标准学会（ANSI）出台了⼀个标准ASCII编码，C语
⾔中的字符就遵循了ASCII编码的⽅式。

详细的ASCLL码表可以参考：https://zh.cppreference.com/w/cpp/language/ascii

### 字符串和\0

C语言中的字符用单引号，而字符串用双引号，字符串的结尾都是默认用'\0'结尾的。

在打印字符串的时候，用`%s`指定，也可以直接打印：

```c
printf("%s\n","hello C");
printf("hello C");
```

### 转义字符

也许在前⾯的代码中你看到 \n , \0 很纳闷时啥。其实在字符中有⼀组特殊的字符是转义字符，转义字符顾名思义：转变原来的意思的字符。

常见转义字符：


- \n	换行符（Newline）	10 (LF)
- \t	水平制表符（Tab）	9 (HT)
- \r	回车符（Carriage Return）	13 (CR)
- \b	退格符（Backspace）	8 (BS)
- \f	换页符（Form Feed）	12 (FF)
- \v	垂直制表符（Vertical Tab）	11 (VT)
- \\	反斜杠（Backslash）	92 (\)
- \'	单引号（Single Quote）	39 (')
- \"	双引号（Double Quote）	34 (")
- \?	问号（防止三字符序列干扰）	63 (?)
- \0	空字符（Null Character）	0 (NUL)
- \xhh	十六进制 ASCII 码字符（如 \x41 代表 'A'）	依照 hh 值
- \ooo	八进制 ASCII 码字符（如 \101 代表 'A'）	依照 ooo 值

转义字符可以详见：https://zh.cppreference.com/w/c/language/escape

### 语句和语句分类

C语⾔的代码是由⼀条⼀条的语句构成的，C语⾔中的语句可为以下五类：

- 空语句
- 表达式语句
- 函数调用语句
- 复合语句
- 控制语句

### 空语句

空语句是最简单的，⼀个分号就是⼀条语句，是空语句。

```c
#include <stdio.h>
int main()
{
    ;//空语句
    return 0;
}
```

空语句，⼀般出现的地⽅是：这⾥需要⼀条语句，但是这个语句不需要做任何事，就可以写⼀个空语句。

### 表达式语句

表达式语句就是在表达式的后边加上分号。如下所⽰：

```c
#include <stdio.h>
int main()
{
    int a = 20;
    int b = 0;
    b = a + 5; //表达式语句
    return 0;
}
```

### 函数调⽤语句

函数调⽤的时候，也会加上分号，就是函数调⽤语句。

```c
#include <stdio.h>
int Add(int x, int y)
{
    return x+y;
}
```

```c
int main()
{
    printf("hehe\n");//函数调⽤语句
    int ret = Add(2, 3);//函数调⽤语句
    return 0;
}
```

### 复合语句

复合语句其实就是前⾯讲过的代码块，成对括号中的代码就构成⼀个代码块，也被称为复合语句。

### 控制语句

控制语句⽤于控制程序的执⾏流程，以实现程序的各种结构⽅式（C语⾔⽀持三种结构：顺序结构、选择结构、循环结构），它们由特定的语句定义符组成，C语⾔有九种控制语句。

可分成以下三类：

- 条件判断语句也叫分⽀语句：if语句、switch语句；
- 循环执⾏语句：do while语句、while语句、for语句；
- 转向语句：break语句、goto语句、continue语句、return语句。
后期会给⼤家⼀⼀介绍控制语句。

### 注释的两种形式

- /**/的形式
- //形式
  
## C语⾔数据类型和变量

### 数据类型

C语言提供丰富的数据类型：

- 字符型
  
```c
char
[signed] char
unsigned char
```
- 整型

```c
//短整型
short [int]
[signed] short [int]
unsigned short [int]

//整型
int
[signed] int
unsigned int

//长整型
long [int]
[signed] long [int]
unsigned long [int]
```

### signed 和unsigned

`signed`表示一个类型带有符号，包括负值。

`unsigned`表示该类型不带有正负号，只有零和正整数。

整数变量声明为unsigned的好处是，同样⻓度的内存能够表⽰的最⼤整数值，增⼤了⼀倍。

注意，C语⾔规定char类型默认是否带有正负号，由当前系统决定。

### 数据类型的取值范围

上述的数据类型很多，尤其数整型类型就有short、int、long、longlong四种，为什么呢？

其实每⼀种数据类型有⾃⼰的取值范围，也就是存储的数值的最⼤值和最⼩值的区间，有了丰富的类
型，我们就可以在适当的场景下去选择适合的类型。如果要查看当前系统上不同数据类型的极限值：

limits.h ⽂件中说明了整型类型的取值范围。

float.h 这个头⽂件中说明浮点型类型的取值范围。

为了代码的可移植性，需要知道某种整数类型的极限值时，应该尽量使⽤这些常量。

• SCHAR_MIN ，SCHAR_MAX ：signedchar的最⼩值和最⼤值。

• SHRT_MIN ，SHRT_MAX ：short的最⼩值和最⼤值。


• INT_MIN ，INT_MAX ：int的最⼩值和最⼤值。

• LONG_MIN ，LONG_MAX ：long的最⼩值和最⼤值。

• LLONG_MIN ，LLONG_MAX ：longlong的最⼩值和最⼤值。

• UCHAR_MAX ：unsignedchar的最⼤值。

• USHRT_MAX ：unsignedshort的最⼤值。

• UINT_MAX ：unsignedint的最⼤值。

• ULONG_MAX ：unsignedlong的最⼤值。

• ULLONG_MAX ：unsignedlonglong的最⼤值。

### 变量的分类

全局变量:在⼤括号外部定义的变量就是全局变量

全局变量的使⽤范围更⼴，整个⼯程中想使⽤，都是有办法使⽤的。

局部变量：在⼤括号内部定义的变量就是局部变量

局部变量的使⽤范围是⽐较局限，只能在⾃⼰所在的局部范围内使⽤的。

```c
#include <stdio.h>
int global = 2023;//全局变量
 
int main()
 {
    int local = 2018;//局部变量
    
    printf("%d\n", local);
    printf("%d\n", global);
    return 0;
 }
```

其实当局部变量和全局变量同名的时候，局部变量优先使⽤。

全局变量在内存的静态区域，局部变量在内存的栈区。

### 运算符

在C语言中，6/4的结果使1.0，而不是1.5.原因在于C语言里面的整数除法是整除，只会返回整数部分，丢弃小鼠部分。如果希望返回的是小数，那在两个运算符号中至少有一个是浮点数
这样C语言就会进行浮点数的除法。

在C语言中的`%`求模运算符号，只能适用于整数，不能用于浮点数。

求模运算的结果的正负号是由第一个运算数决定的。

```c
#include <stdio.h>
 int main()
 {
    printf("%d\n", 11 % -5); // 1
    printf("%d\n",-11 % -5); // -1
    printf("%d\n",-11 % 5); // -1
    return 0;
 }
```

可以连续赋值：
```c
 c = b = a + 3;//连续赋值，从右向左依次赋值的。
```
C语⾔虽然⽀持这种连续赋值，但是写出的代码不容易理解，建议还是拆开来写，这样⽅便观察代码的
执⾏细节。

C语言中可以进行复合赋值语句：

```c
int a = 10;
a += 3;
a = a + 3;
```

常见复合赋值语句：
```
+=        
*=        
-=
 /=        
 //
 
>>=       
&=        
<<=
 |=        
%=
 ^=
```

有两个操作数的是双目运算符，只有一个操作数的是单目运算符。

例如：`++、--`就是单目运算符。

前置的++和后置的++并不一样，前置的是先加1之后使用，而后置的是先使用之后加1。

### 强制类型转化

```
int a = 3;
float b = (float)a;
```

俗话说，强扭的⽠不甜，我们使⽤强制类型转换都是万不得已的时候使⽤，如果不需要强制类型转化
就能实现代码，这样⾃然更好的。

## scanf和printf的介绍

### printf

printf不会再输出的内容中添加换行符，需要插入换行符。

printf() 可以在输出⽂本中指定占位符。

所谓“占位符”，就是这个位置可以⽤其他值代⼊。

```C
// 输出 There are 3 apples 
#include <stdio.h>
 int main()
 {
    printf("There are %d apples\n", 3);
    return 0;
 }
```

上⾯⽰例中，
There are %d apples\n 是输出⽂本，⾥⾯的`%d `就是占位符，表⽰这个位置要
⽤其他值来替换。占位符的第⼀个字符⼀律为百分号%
，第⼆个字符表⽰占位符的类型，⾥代⼊的值必须是⼀个整数。
`printf() `的第⼆个参数就是替换占位符的值，上⾯的例⼦是整数3
替换就是There are 3 apples 。

`%s`代表的是字符串。

### 限定宽度

printf() 允许限定占位符的最⼩宽度。

```c
#include <stdio.h>
 int main()
 {
    printf("%5d\n", 123); // 输出为"  123" 
    return 0;
 }
```

上⾯⽰例中，%5d 表⽰这个占位符的宽度⾄少为5位。如果不满5位，对应的值的前⾯会添加空格。
输出的值默认是右对⻬，即输出内容前⾯会有空格；如果希望改成左对⻬，在输出内容后⾯添加空
格，可以在占位符的%的后⾯插⼊⼀个-号。

### scanf

当我们需要输入一个值的时候，就可以使用scanf。

scanf() 函数⽤于读取⽤⼾的键盘输⼊。

scanf() 的语法跟printf() 类似。

```c
scanf("%d", &i);
```

变量前面必须加上&运算符，因为scanf()传递的不是值，而是地址，急变量i的地址指向的用户输入的值。

如果变量是指针变量，那么就不用&运算符。