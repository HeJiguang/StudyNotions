# vector数组

 string是针对字符类型的数组设计的，vector是针对所有类型的数组实现的。

## 创建vector

```cpp
vector<int> arrl;
```

## 放入元素

```cpp
arr1.push_back(1);
arr1.push_back(2);
```

## 迭代器访问

```cpp
vector<int>::iterator it1 = v1.begin();
while (it1 != v1.end()){
    cout << *it1 << " ";
    ++it1;
}
```

## 下标访问

```cpp
for(size_t i = 0;i < v1.size();i++){
    cout << v1[i] << " ";
}
```

## auto访问
```cpp
for (auto e : v1){
    cout << e << " ";
}
```



