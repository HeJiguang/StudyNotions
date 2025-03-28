# 

## 1. 链表的使用


## 2. 链表的简单实现

### 2.1 链表的框架：

```cpp
namespace sinlist {
	template<class T>
	struct ListNode {
		ListNode<T>* _next;
		ListNode<T>* _prev;

		T _data;
	};

	template<class T>
	class list {
		typedef ListNode<T> Node;

	public:
		//构造函数
		list() {
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
		}
		//尾插
		void push_back(const T& x) {
			Node* newnode = new Node;
			Node* tail = _head->_prev;

			newnode->_data = x;
			newnode->_next = _head;
			newnode->_prev = tail;

			tail->_next = newnode;
			_head->_prev = newnode;
		}
	
	private:
		Node* _head;

	};
}
```

这里实现一个简单的链表的框架，一个模板结构ListNode，其中有两个指针，next用来指向下一个节点，prev用来指向上一个节点。还有一个data用来存储数据。

实现一个list类，其中包含一个头节点_head。实现构造函数与尾插函数。

### 2.2 迭代器

接着我们需要实现迭代器。迭代器不可以直接为Node*，需要实现为一个类，并重载需要的运算符。

迭代器可以定义为list的内部类，也可以定义为一个独立的类。

```cpp
 	template<class T>
	class Listiterator {
		typedef ListNode<T> Node;
		typedef ListIterator<T> Self;

		Node* _node;

		ListIterator(Node* node)
			:_node(node) {

		}

	};
```

这样就实现了一个迭代器的框架，接着写一个