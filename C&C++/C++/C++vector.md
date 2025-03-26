# vector数组的使用与简单实现

 string是针对字符类型的数组设计的，vector是针对所有类型的数组实现的。

文中源码详见：https://github.com/HeJiguang/StudyNotions/blob/main/C%26C%2B%2B/C%2B%2B/vector.cpp

## 1. 基本操作

## 1.1 创建vector

```cpp
vector<int> arrl;
```

## 1.2 放入元素

```cpp
arr1.push_back(1);
arr1.push_back(2);
```

## 1.3 迭代器访问

```cpp
vector<int>::iterator it1 = v1.begin();
while (it1 != v1.end()){
    cout << *it1 << " ";
    ++it1;
}
```

## 1.4 下标访问

```cpp
for(size_t i = 0;i < v1.size();i++){
    cout << v1[i] << " ";
}
```

## 1.5 auto访问
```cpp
for (auto e : v1){
    cout << e << " ";
}
```

## 1.6 sort排序

sort是一个函数模板，需要的参数是容器的迭代器。

包含sort的头文件：

```cpp
#include <algorithm>
```

sort需要传入的参数是迭代器：

```cpp
vector<int> v1 = {3,1,4,2,5};

// 默认升序排序
sort(v1.begin(), v1.end());

// 使用greater实现降序排序
sort(v1.begin(), v1.end(), greater<int>());
```

默认升序排序，传入的greater是一个函数对象，可以实现降序排序。

### 扩展排序应用

1. 自定义比较函数：
```cpp
// 按绝对值排序
sort(v1.begin(), v1.end(), [](int a, int b) {
    return abs(a) < abs(b);
});
```

2. 稳定性说明：
- std::sort 使用快速排序（平均O(n log n)）
- 非稳定排序，相等元素可能改变相对顺序
- 需要稳定排序时使用stable_sort

3. 性能特征：
- 元素超过阈值时使用堆排序（防止O(n²)）
- 小范围使用插入排序
- 要求随机访问迭代器，链表不可用


## vector模拟实现
下面让我来模拟实现一个vector数组：

```cpp
namespace sin {
	template<class T>
	class vector {
	public:
		typedef T* iterator;

		// 析构函数
		~vector() {
			if (_start) {
				delete[] start;
				_start = _finish = _end_of_storage = nullptr;
			}
		}

		// 扩容
		void reserve(size_t n) {
			// 时间复杂度分析：
			// 1. 不发生扩容：O(1)
			// 2. 发生扩容：O(n) 需要拷贝所有元素
			size_t oldsize = size();
			T* tmp = new T[n];


			if (n > capacity()) {
				memcpy(tmp, _start, sizeof(T) * size());
				delete[] _start;
			}

			_start = tmp;
			_finish = _start + oldsize;
			_end_of_storage = _start + n;
		}

		// 计算容器容量
		size_t capacity() {
			return _end_of_storage - _start;
		}

		// 获取数组的长度
		size_t size() {
			return _finish - _start;
		}

		void  push_back(const T& x) {
			/* 均摊时间复杂度分析：
			 * 采用二倍扩容策略时，n次push_back操作的总时间复杂度为O(n)
			 * 单次操作均摊时间复杂度为O(1)
			 */
			if (_finish == _end_of_storage) {
				size_t newcapacity = capacity() == 0 ? 4 : capacity() * 2;
				reserve(newcapacity);
			}

			// 放入数据
			*_finish = x;
			++_finish;
		}
		
	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;

	};
}
```

这里是一个简单的框架，将T*类型起一个别名iterator。

有三个成员变量，开始的地址_start、结尾的地址_finish与空间最后的位置_end_of_storage。


实现了最基础的push_back函数，与几个工具函数，如size、capacity、reserve，可以实现放入数据，当发现空寂不够的时候，进行扩容。


接下来实现用下标访问，需要重载操作符[]。

```cpp
//下标访问
T& operator[](size_t i) {
	// 检查越界情况
	assert(i < size()); 

	return _start[i];
}
```

写一个简单的test，测试现在写好的功能：

```cpp
void test01() {
	sinnt::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	for (size_t i = 0; i < v1.size();i++) {
		cout << v1[i] << "";
	}
	cout << endl;
}
```

运行后打印出1234，表明到这里vetor的构造是正确的。

### 迭代器iterator

这里还没有实现迭代器，只是用一个指针作为迭代器，故没法使用范围for（如1.5）去对数组进行遍历。

添加下面代码，实现迭代器的begin和end

```cpp

iterator begin(){
    return _start;
}

iterator end(){
    return _finish;
}
```

实现之后就可以正常的使用迭代器，下面是简单的测试：

```cpp
void test02() {
	sinnt::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	//迭代器遍历
	sinnt::vector<int>::iterator it = v1.begin();
	while (it != v1.end()) {
		cout << *it << " ";
        ++it; 
	}
	cout << endl;

	//auto与for遍历

	for (auto e : v1) {
		cout << e << " ";
	}
}
```

做了迭代器的模拟，可以加深对于迭代器的理解。

这里的`sinnt::vector<int>::iterator`其实就是`int *`，在语法上面是等价的。

实现尾删：

```cpp
		// 实现尾删
		void pop_back() {
			assert(size() > 0);

			--_finish;
		}
```

实现插入：

原来vector数组使用插入：

```cpp
v1.insert(v1.begin() + 2,20);
```

这样就在开始后的第二个位置插入了20，第一个参数是一个迭代器，第二个参数是传入的内容。

实现在一个迭代去后面插入内容：

```cpp
		// 插入
		// 时间复杂度：O(n) 需要移动后续所有元素
void insert(iterator pos, const T& x) {
			if (_finish == _end_of_storage) {
				size_t len = pos - _start;

				size_t newcapacity = capacity() == 0 ? 4 : capacity() * 2;
				reserve(newcapacity);

				pos += len;
			 }

			iterator end = _finish - 1;
			while (end >= pos) {
				*(end + 1) = *end;
				--end;
			}
			*pos = x;
			++_finish;
		}
```

通过插入的代码，我们会发现，当产生扩容的时候，传入的迭代器pos可能会失效，所以可能失效的迭代器不能再使用。

### 迭代器失效机制详解

vector的迭代器失效主要发生在以下场景：
1. **插入操作**：当引起内存重新分配时，所有迭代器都会失效；未重新分配时，插入位置之后的迭代器失效
2. **删除操作**：被删除元素及其之后的所有迭代器都会失效
3. **reserve/resize**：可能引起内存重新分配导致所有迭代器失效

标准库的insert会返回新的有效迭代器：
```cpp
// 标准库insert原型
iterator insert(iterator position, const T& x);
```

正确使用方式：
```cpp
// 插入后需要更新迭代器
it = vec.insert(it, new_value);
++it;  // 跳过新插入的元素
```

**失效迭代器特征**：
- 指向被释放的内存（野指针）
- 比较操作可能返回错误结果
- 解引用会导致未定义行为

建议在修改容器后：
1. 重新获取begin()/end()迭代器
2. 避免保存旧的迭代器引用
3. 使用索引访问进行复杂操作

string的迭代器失效原理与vector相同，但由于字符串操作的特性，更推荐使用索引访问。

同理可以实现erase，删除一个位置的内容：

```cpp
		// 删除一个位置的内容
		// 时间复杂度：O(n) 需要移动后续所有元素
void erase(iterator pos) {
			assert(pos >= _start);
			assert(pos < _finish);

			iterator it = pos + 1;
			while (it != _finish) {
				*(it - 1) = *it;
				++it;
			}

			--_finish;
		}
```

erase之后的这个指针是不能再使用的。

 
### vector的拷贝构造

```cpp
		// 拷贝构造
		vector(const vector<T>& v) {
			for (auto e : v) {
				push_back(e);
			}
		}
```

在没有写构造函数的时候，编译器会帮我们实现一个默认的构造函数，但是一旦我们实现了一个构造函数，编译器就不会生成构造函数。所以这里只实现一个拷贝构造函数，会让我们这里的vector类没有默认构造函数。

所以我们需要再写一个默认构造函数，或者要求编译器生成默认的：

```cpp
vector() = default;
```

同时，我们也需要重构=号实现拷贝构造。

```cpp
void swap(vector<T>& v){
	std::swap(_start,v._start);
	std::swap(_finish,v._finish);
	std::swap(_end_of_storage,v._end_of_storage);
}


vector<T>& operator=(vector<T> v){
	swap(v);
	return *this;
}
```

