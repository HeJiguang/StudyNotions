# C++复习：C++入门

## 命名空间

Bjarne Stroustrup解决了大量CPP的问题。

比如命名冲突，我们自己写的代码可以和C的标准库中的命名冲突，导致报错。所以，
C++引入了命名空间`namesapce`去解决命名冲突的问题。

在C语言中，有局部优先原则，`::`可以访问全局变量：

```cpp
int a = 10

int main(){
    int a = 5;
    cout << a << endl;
    cout << ::a << endl;
    return 0;
}
```
这里输出的值是5和10。

`::`是指定命名空间，这里的`::a`实质上就是指定全局命名空间。

编译默认的查找，首先是查找当前的局部变量，在找不到之后会去全局域查找，不会去命名空间中查找。

```cpp
namespace sin
{
    int hejiguang;
}

int main(){
    cout << hejiguang;
    return 0;
}
```

这里会报错，而加入指定域之后`sin::hejiguang`，可以查找到在域sin中的内容。

命名空间中可以定义变量，也可以定义函数和类型。

命名空间可以嵌套，但不是很常用，例如：

```cpp
struct sin::fz::Node pnode;
```

### 使用命名空间

不同的文件中可以定义同名的命名空间，多个文件中的命名空间可以合并。这样可以解决当代码量过大导致的命名空间过多的问题。

命名空间使用`using namespace`可以展开，这样在文件中就可以不再指定命名空间。常见的是：

```cpp
using namespace std;
```

当展开命名空间之后，会先在局部找，之后在全局找，最后在命名空间包含的部分寻找。

多个命名空间同时展开，如果其中有包含同名命名空间，还是可能导致命名冲突的问题。所以命名空间不要随便展开。

如何在命名空间中有变量名需要经常使用，也可以不展开整个命名空间，只展开某一个：

```cpp
using sin::hejiguang;
```

这样只展开了sin中的hejiguang这一个变量名，而没有将sin中的变量全部展开。

关于域的概念，有全局域，局部域，命名空间域，类域等等。作用都是帮助C++查找变量。

## iostream

C++有一套新的输入输出流，需要包含的头文件时iostream。

```cpp
#include<iostream>
```

输出一个内容：

```cpp
double i = 1.1;
cout << i << endl;
```

这里的cout是将后面的内容打印在屏幕中，`printf`需要自己指定类型。相比之下cout更加方便好用。

这里的`cout`在官方库命名空间`std`中。

所以一般程序开头出现的
```cpp
using namespace std;
```

就是在包含std命名空间。也可以不全部展开，只展开`cout`

```cpp
using std::cout;
```

而c++中的cin是输入函数：

```cpp
std::cin >> i;
```
