# C++STL标准库学习STring

## 什么是STL

STL（Standard Template Library）是C++标准库的核心组件，它提供了一系列模板类和函数，实现了常用的数据结构和算法。STL的设计理念是"泛型编程"，通过模板技术实现了代码的复用和通用性。

## STL的六大组件简介

1. 容器（Containers）：用于存储数据的类模板
2. 算法（Algorithms）：用于处理容器中数据的函数模板
3. 迭代器（Iterators）：用于遍历容器中元素的类模板
4. 仿函数（Functors）：行为类似函数的类模板
5. 适配器（Adapters）：用于修改容器或函数接口的类模板
6. 分配器（Allocators）：用于内存管理的类模板

## String

String是C++标准库中用于处理字符串的类，它封装了字符数组的操作，提供了丰富的字符串处理功能。

### String的特点

1. 动态扩容：String可以根据需要自动调整大小
2. 面向对象：将数据和方法封装在一起
3. 安全性：提供了边界检查，防止越界访问
4. 丰富的接口：提供了大量字符串操作方法

### String的构造函数

String提供了多种构造函数：

```cpp
// 默认构造函数
string str1;  // 创建一个空字符串

// 拷贝构造函数
string str2(str1);  // 使用str1构造str2
string str3 = str1; // 另一种拷贝构造方式

// 带参构造函数
string str4("Hello");           // 使用C风格字符串构造
string str5(5, 'A');           // 构造包含5个'A'的字符串
string str6(str1, 0, 3);       // 从str1的0位置开始，取3个字符
```

### String的常用操作

1. 访问元素：
```cpp
string str = "Hello";
char c1 = str[0];     // 使用[]操作符
char c2 = str.at(0);  // 使用at()方法（带边界检查）
```

2. 修改操作：
```cpp
string str = "Hello";
str += " World";      // 字符串拼接
str.append("!");      // 追加字符串
str.push_back('!');   // 追加单个字符
str.insert(5, " ");   // 在指定位置插入
str.erase(0, 5);      // 删除指定范围的字符
```

### String的遍历

#### 1. 下标+[]遍历
```cpp
string str = "Hello";
for(size_t i = 0; i < str.size(); i++) {
    cout << str[i] << " ";
}
```

#### 2. 迭代器遍历
```cpp
string str = "Hello";
// 正向迭代器
for(string::iterator it = str.begin(); it != str.end(); it++) {
    cout << *it << " ";
}

// 范围for循环（C++11）
for(char c : str) {
    cout << c << " ";
}
```

#### 3. 反向迭代器遍历
```cpp
string str = "Hello";
for(string::reverse_iterator it = str.rbegin(); it != str.rend(); it++) {
    cout << *it << " ";
}
```

### 迭代器

String的迭代器分为const_iterator和iterator。

其中，iterator是可读可写的，而const_iterator是只能读而不能写的。

#### 迭代器的类型

1. 正向迭代器（iterator）：
```cpp
string::iterator it;  // 可读可写
```

2. 常量正向迭代器（const_iterator）：
```cpp
string::const_iterator cit;  // 只读
```

3. 反向迭代器（reverse_iterator）：
```cpp
string::reverse_iterator rit;  // 可读可写
```

4. 常量反向迭代器（const_reverse_iterator）：
```cpp
string::const_reverse_iterator crit;  // 只读
```

#### 迭代器的基本操作

```cpp
string str = "Hello World";

// 获取迭代器
string::iterator it = str.begin();    // 指向第一个元素
string::iterator end = str.end();     // 指向最后一个元素的下一个位置

// 迭代器的基本操作
*it;           // 解引用，获取当前元素
++it;          // 向前移动
--it;          // 向后移动
it + 2;        // 向前移动2个位置
it - 2;        // 向后移动2个位置

// 迭代器比较
it1 == it2;    // 判断两个迭代器是否相等
it1 != it2;    // 判断两个迭代器是否不相等
it1 < it2;     // 判断迭代器位置先后
```

#### 迭代器的使用示例

```cpp
string str = "Hello World";

// 使用迭代器遍历并修改
for(string::iterator it = str.begin(); it != str.end(); ++it) {
    *it = toupper(*it);  // 将字符转换为大写
}

// 使用const迭代器遍历（只读）
for(string::const_iterator it = str.begin(); it != str.end(); ++it) {
    cout << *it << " ";  // 只能读取，不能修改
}

// 使用反向迭代器从后向前遍历
for(string::reverse_iterator it = str.rbegin(); it != str.rend(); ++it) {
    cout << *it << " ";
}
```

#### 迭代器的注意事项

1. 迭代器失效：
   - 当对string进行插入或删除操作时，可能会导致迭代器失效
   - 建议在修改操作后重新获取迭代器

2. 边界检查：
   - 使用迭代器时要注意不要越界
   - 解引用前确保迭代器有效

3. 性能考虑：
   - 对于简单的遍历，使用下标操作符[]可能比迭代器更快
   - 但在使用STL算法时，迭代器是必需的

### String的常用方法

1. 容量相关：
```cpp
str.size();      // 返回字符串长度
str.length();    // 返回字符串长度
str.empty();     // 判断是否为空
str.capacity();  // 返回当前容量
str.reserve(100); // 预分配空间
```

2. 查找相关：
```cpp
str.find("lo");      // 查找子串
str.rfind("lo");     // 从后向前查找
str.find_first_of("aeiou");    // 查找第一个元音字母
str.find_last_of("aeiou");     // 查找最后一个元音字母
```

3. 子串操作：
```cpp
str.substr(0, 5);    // 获取子串
str.compare("Hello"); // 字符串比较
```

