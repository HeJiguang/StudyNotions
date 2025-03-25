# include<iostream>
# include<assert.h>

using namespace std;

namespace vector {
	template<class T>
	class vector {
	public:
		typedef T* iterator;

		// 拷贝构造
		vector(const vector<T>& v) {
			for (auto e : v) {
				push_back(e);
			}
		}

		void swap(vector<T>& v) {
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}


		vector<T>& operator=(vector<T> v) {
			swap(v);
			return *this;
		}
		//强制编译器生成默认构造函数。
		vector() = default;


		// 析构函数
		~vector() {
			if (_start) {
				delete[] _start;
				_start = _finish = _end_of_storage = nullptr;
			}
		}

		// 扩容
		void reserve(size_t n) {
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


		//下标访问
		T& operator[](size_t i) {
			// 检查越界情况
			assert(i < size()); 

			return _start[i];

		}
		
		//实现完全迭代器
		iterator begin() {
			return _start;
		}

		iterator end() {
			return _finish;
		}


		void  push_back(const T& x) {

			// 如果发现数据占满空间，则扩容
			if (_finish == _end_of_storage) {
				size_t newcapacity = capacity() == 0 ? 4 : capacity() * 2;
				reserve(newcapacity);
			}

			// 放入数据
			*_finish = x;
			++_finish;
		}
		
		// 删除一个位置的内容
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

		// 插入
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

		// 实现尾删
		void pop_back() {
			assert(size() > 0);

			--_finish;
		}


	private:
		iterator _start = nullptr;
		iterator _finish = nullptr;
		iterator _end_of_storage = nullptr;

	};
}


void test01() {
	vector::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	for (size_t i = 0; i < v1.size();i++) {
		cout << v1[i] << "";
	}
	cout << endl;
}

void test02() {
	vector::vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);

	//迭代器遍历
	vector::vector<int>::iterator it = v1.begin();
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

int main() {
	test02();
	return 0;
}