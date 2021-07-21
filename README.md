# C++实现常用数据结构

记录使用C++实现常用数据结构的学习过程。

## 参考

[sky12345数据结构与算法系列]: https://www.cnblogs.com/skywang12345/p/3603935.html

## 目录

- [C++实现常用数据结构](#c----------)
  * [参考](#--)
  * [目录](#--)
  * [栈](#-)
    + [1 栈的示意图](#1------)
    + [2 出栈](#2---)
    + [3 入栈](#3---)
    + [4 C++实现](#4-c----)
  * [双向循环链表](#------)
    + [1 双向循环链表示意图](#1----------)
    + [2 删除节点](#2-----)
    + [3 添加节点](#3-----)
    + [4 C++实现](#4-c-----1)
  * [队列](#--)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>



## 1 栈     

栈（stack）是一种线性的数据结构，可以使用数组或链表实现。它具有以下两种特点：    

- 按照“后进先出”的顺序访问；   
- 只能对栈顶元素进行操作。   

栈的三种常用操作：   

1. push：向栈顶增加元素；   
2. pop：返回栈顶元素并删除该元素；    
3. top：返回栈顶元素，不删除。   

### 1.1 栈的示意图    

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/390eceb34d5f56275e38d266d29b026ac303176f/images/Stack/1.%E6%A0%88%E7%9A%84%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

栈中的数据依次是 30→20→10。    

### 1.2 出栈     

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/c2480c0086fe23fe0e84189652e599c924813b6e/images/Stack/2.%E5%87%BA%E6%A0%88%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

**出栈前**：栈顶元素是30。此时，栈中的元素依次是 30→20→10。            

**出栈后**：30出栈之后，栈顶元素变成20。此时，栈中的元素依次是 20→10。     

### 1.3 入栈     

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/aacdd4254c1a7b03e4fe3b49d4267f1cb53f621a/images/Stack/3.%E5%85%A5%E6%A0%88%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

**入栈前**：栈顶元素是20。此时，栈中的元素依次是 20→10。     
**入栈后**：40入栈之后，栈顶元素变成40。此时，栈中的元素依次是 40→20→10。     

### 1.4 C++实现

使用模板实现存储任意类型数据，参考文件`MyStack.h`。因为使用了模板，所以实现也放在头文件中，如果实现放在`.cpp`文件中，会报链接错误。

原因参考[C++模板的定义是否只能放在头文件中？](https://blog.csdn.net/imred/article/details/80261632)

#### 1.4.1 需求明确

1. 一个变量`size`存储栈的大小；
2. 一个变量`top`指向栈顶；
3. 一个size大小的内存空间;
4. 入栈操作`push`;
5. 出栈操作`pop`；
6. 取栈顶元素`top`；
7. 检查栈是否为空的函数`isEmpty`；
8. 检查栈是否满的函数`isFull`。

#### 1.4.2 栈的类定义

```c++
template<class _Ty>
class MyStack
{
public:
	MyStack(int size);
	virtual ~MyStack();
	// 1.入栈
	void push(_Ty element);
	// 2.出栈
	_Ty pop(void);
	// 3.获取栈顶元素
	_Ty top(void);
	// 4.检查栈是否为空
	bool isEmpty(void) const;
	// 5.检查栈是否满了
	bool isFull(void) const;

private:
	_Ty *stack;
	int  size;
	int  m_top;
};
```

- 使用模板，实现存储任意类型的元素；
- 提供输入参数为size的构造函数，用于创建size大小的栈；

#### 1.4.3 构造函数

```c++
template<class _Ty>
MyStack<_Ty>::MyStack(int size)
{
	this->size = size;
	stack = new _Ty[size];
	m_top = 0;
}
```

#### 1.4.4 析构函数

```c++
template<class _Ty>
inline MyStack<_Ty>::~MyStack()
{
	delete[] stack;
}
```

#### 1.4.5 入栈（push）

```c++
template<class _Ty>
void MyStack<_Ty>::push(_Ty element)
{
	if (isFull())
	{
		throw MyException("The stack space is full.");
	}
	stack[m_top++] = element;
}
```

#### 1.4.6 出栈（pop）

```c++
template<class _Ty>
_Ty MyStack<_Ty>::pop(void)
{
	if (isEmpty())
		throw MyException("The stack is empty.");

	return stack[--m_top];
}
```

#### 1.4.7 取栈顶元素（top）

```c++
template<class _Ty>
_Ty MyStack<_Ty>::top(void)
{
	if (isEmpty())
		throw MyException("The stack is empty.");

	return stack[m_top - 1];
}
```

#### 1.4.8 判断栈是否空

```c++
template<class _Ty>
inline bool MyStack<_Ty>::isEmpty(void) const
{
	return m_top == 0;
}
```

#### 1.4.9 判断栈是否满

```c++
template<class _Ty>
inline bool MyStack<_Ty>::isFull(void) const
{
	return m_top == size;
}
```



## 2 单链表

单链表是一种链式数据结构，许多复杂的数据结构都或多或少使用到单链表的特性，如二叉搜索树（BSTree）。

单链表中的数据是以结点来表示的，每个结点由元素和指向后继结点的指针构成。单链表相对于线性结构，如数组，优点是插入及删除灵活，缺点是随机访问能力弱。

### 2.1 单链表示意图

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/5b9a0beb4f86c40b1b405c4d06ce0ffedbd555d9/images/SingleList/1.%E5%8D%95%E9%93%BE%E8%A1%A8%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

### 2.2 插入结点

**插入元素“25”前：**10→20→30→40

**插入元素“25”后：**10→20→25→30→40

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/5b9a0beb4f86c40b1b405c4d06ce0ffedbd555d9/images/SingleList/2.%E5%8D%95%E9%93%BE%E8%A1%A8%E6%8F%92%E5%85%A5.svg)

### 2.3 删除结点

**删除元素“25”前：**10→20→25→30→40

**删除元素“25”后：**10→20→30→40

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/5b9a0beb4f86c40b1b405c4d06ce0ffedbd555d9/images/SingleList/3.%E5%8D%95%E9%93%BE%E8%A1%A8%E5%88%A0%E9%99%A4.svg)

### 2.4 C++实现

#### 2.4.1 需求明确

1. 一个用来存储数据的变量`value`；
2. 一个指向后继结点的指针`next`；
3. 一个用来存储链表大小的变量`m_size`；
4. 在链表尾部添加一个结点的方法`push_back()`；
5. 在链表头部添加一个结点的方法`insert_front()`；
6. 在结点的值比被插入结点的值大前插入结点的方法`insert()`；
7. 删除一个结点的方法`remove()`；
8. 删除全部值为value的方法`remove_all_of()`；
9. 销毁链表的方法`destroy()`；
10. 获取链表结点数量的方法`size()`；
11. 获取链表最小值的方法`min()`；
12. 获取链表最大值的方法`max()`；
13. 查询是否包含某个结点的方法`contains()`；

#### 2.4.2 结点定义

```c++
template <class T>
class Node
{
public:
    T value;	// 结点的值
    Node* next; // 指向下一个结点的指针
    Node() : next(nullptr) {}	
    Node(T val, Node* p = nullptr) : value(val), next(p) {}	// 新建结点，下一个结点为p
};
```

#### 2.4.3 链表定义

```c++
template <class T>
class LinkList
{
private:
    int m_size;		// 结点数量
    Node<T>* head;	// 表头
    Node<T>* tail;	// 表尾

    class List_Iterator		// 内置迭代器类
    {
    public:
        List_Iterator(Node<T>* p = nullptr) : ptr(p) {}	// 迭代器构造函数

        Node<T>& operator*() const		// 重载 * 运算符
        {
            return *ptr;
        }
        Node<T>* operator->() const		// 重载->运算符
        {
            return ptr;
        }
        List_Iterator& operator++()		// 重载前置自增运算符
        {
            ptr = ptr->next;
            return *this;
        }

        List_Iterator operator++(int)	// 重载后置自增运算符
        {
            Node<T>* tmp = ptr;
            ++(*this);
            return List_Iterator(tmp);
        }

        bool operator==(const List_Iterator& t) const	// 重载==运算符
        {
            return t.ptr == this->ptr;
        }

        bool operator!=(const List_Iterator& t) const	// 重载!=运算符
        {
            return t.ptr != this->ptr;
        }

    private:
        Node<T>* ptr;	// 结点指针
    };

public:
    using iterator = List_Iterator;

    LinkList();
    ~LinkList();
    
    // 1.后插
    void push_back(const T& value);
    // 2.前插
    void insert_front(const T& value);
    // 3.在第一个值比value大的节点前插入
    void insert(const T& value);
    // 4.删除第一个值为value的结点
    void remove(const T& value);
    // 5.删除所有值为value的结点
    void remove_all_of(const T& value);
    // 6.销毁链表
    void destroy();
    int  size() const;
    T min() const;
    T max() const;
    bool contains(const T& value) const;
    void print(ostream& os = cout) const;

    iterator begin() const	// 迭代器头
    {
        return List_Iterator(head);
    }
    iterator end() const	// 迭代器尾
    {
        return List_Iterator(tail->next);
    }
};
```

#### 2.4.4 后插法

```c++
template<class T>
void LinkList<T>::push_back(const T& value)
{
    if (!head)
    {
        head = new Node<T>(value);
        tail = head;
    }
    else
    {
        tail->next = new Node<T>(value);
        tail = tail->next;
    }
    m_size++;
}	
```

#### 2.4.5 前插法

```c++
template<class T>
void LinkList<T>::insert_front(const T& value)
{
    if (!head)
    {
        head = new Node<T>(value);
        tail = head;
    }
    else
    {
        Node<T>* node = new Node<T>(value, head);
        head = node;
    }
}
```

#### 2.4.6 升序插入

```c++
template<class T>
void LinkList<T>::insert(const T& value)
{
    Node<T>** tmp;

    for (tmp = &head; *tmp; tmp = &(*tmp)->next)		// 使用二级指针一次遍历完成插入
    {
        if ((*tmp)->value > value)
        {
            Node<T>* node = new Node<T>(value, *tmp);
            (*tmp) = node;
            m_size++;
            return;
        }
    }
    (*tmp) = new Node<T>(value, nullptr);
    tail = *tmp;
    m_size++;
}
```

#### 2.4.7 删除结点

```c++
template<class T>
void LinkList<T>::remove(const T& value)
{
    Node<T>** tmp;
    for (tmp = &head; *tmp; tmp = &(*tmp)->next)	// 使用二级指针，遍历一次删除
    {
        if ((*tmp)->value == value)
        {
            Node<T>* node = *tmp;
            *tmp = (*tmp)->next;
            delete node;
            m_size--;
            return;
        }
    }
}
```

#### 2.4.8 删除所有值为value的结点

```c++
template<class T>
void LinkList<T>::remove_all_of(const T& value)
{
    Node<T>** tmp;
    for (tmp = &head; *tmp;)
    {
        if ((*tmp)->value == value)
        {
            Node<T>* node = *tmp;
            *tmp = (*tmp)->next;
            delete node;
            m_size--;
        }
        else
            tmp = &(*tmp)->next;
    }
}
```

#### 2.4.9 销毁链表

```c++
template<class T>
void LinkList<T>::destroy()
{
    while (head)
    {
        Node<T>* tmp = head;
        head = head->next;
        delete tmp;
    }
    head = nullptr;
}
```



## 二 双向循环链表

双向循环链表，顾名思义，每个节点都有两个指针，一个指向前驱节点，一个指向后继节点。头节点的前驱节点是尾节点，尾节点的后继节点是头节点。双向循环链表相较于单链表更具灵活性。

### 1 双向循环链表示意图

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/a48fdd040fc41959eb608432fce9fc3ed671a8b1/images/DoubleLinkList/%E5%8F%8C%E5%90%91%E5%BE%AA%E7%8E%AF%E9%93%BE%E8%A1%A8%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

### 2 删除节点

![img](https://raw.githubusercontent.com/Ryzen7-3700X/Cplusplus-DataStructure/a48fdd040fc41959eb608432fce9fc3ed671a8b1/images/DoubleLinkList/%E5%88%A0%E9%99%A4%E8%8A%82%E7%82%B9%E7%A4%BA%E6%84%8F%E5%9B%BE.svg)

删除"节点20"。
**删除之前**："节点10"的后继节点为"节点20"，"节点30" 的前继节点为"节点20"。"节点30"的后继节点为"节点40"，"节点40" 的前继节点为"节点30"。
**删除之后**："节点20"的后继节点为"节点40"，"节点40" 的前继节点为"节点20"。

### 3 添加节点

![img](https://images0.cnblogs.com/i/497634/201403/241342164043381.jpg)

在"节点10"与"节点20"之间添加"节点15"。
**添加之前**："节点10"的后继节点为"节点20"，"节点20" 的前继节点为"节点10"。
**添加之后**："节点10"的后继节点为"节点15"，"节点15" 的前继节点为"节点10"。"节点15"的后继节点为"节点20"，"节点20" 的前继节点为"节点15"。

### 4 C++实现

使用模板实现存储任意类型数据，参考文件MyDoubleLink.h。

## 三 二叉查找树

### 1 树的介绍
#### 1.1 树的定义
树是一种数据结构，由n（n ≥ 1）个有限节点组成一个具有层次关系的集合。
![image](https://user-images.githubusercontent.com/34436659/121865569-b2fd4880-cd30-11eb-988b-c648742e5024.png)
把它叫做树，是因为它看起来像一棵倒挂的树，也就是根朝上，叶子朝下。它具有以下特点：
* 每个节点有零个或多个子节点；
* 没有父节点的节点称作根节点；
* 每一个非根节点有且仅有一个父节点；
* 除了根节点外，每个子节点可以分为多个不相交的子树。

#### 1.2 树的基本术语
若一个节点有子树，那么该节点成为子树根的“双亲”，子树的根是该节点的“孩子”。有相同双亲的节点互为“兄弟节点”。一个节点的所有子树上的任何节点都是该节点的后裔。从根节点到某个节点的路径上的所有节点都是该节点的祖先。
* **节点的度：** 节点拥有的子树的数目；
* **叶子：** 度为零的节点；
* **分支节点：** 度不为零的节点；
* **树的度：** 树中节点的最大的度；
* **层次：** 根节点的层次为1，其余节点的层次等于该节点的双亲节点的层次加1；
* **树的高度：** 树中节点的最大层次；
* **无序树：** 树中节点的各子树之间的次序是不重要的，可以交换位置；
* **有序树：** 树中节点的各子树之间的次序是重要的，不可以交换位置；
* **森林：** 零个或多个不相交的树组成。对森林加上一个树根，森林即成为树，删去树根，树即成为森林。

### 2 二叉树的介绍
#### 2.1 二叉树的定义
二叉树是每个节点最多有两个子树的结构，它有五种基本形态：
* 空二叉树；
* 只有树根；
* 根只有左子树；
* 根只有右子树；
* 根有左右子树。

#### 2.2 二叉树的性质
二叉树有以下几个基本性质：
* **性质1：** 二叉树的第i层最多有2<sup>i-1</sup>个节点（i≥1）；
* **性质2：** 深度为k的二叉树最多有2<sup>k</sup>-1个节点（k≥1）；
* **性质3：** 包含n个节点的二叉树的最低高度为log<sub>2</sub>(n+1)，向上取整；
* **性质4：** 在任意一棵二叉树中，若终端节点的个数为n<sub>0</sub>，度为2的节点数为n<sub>2</sub>，则n<sub>0</sub>=n<sub>2</sub>+1。

##### 证明性质1
**证明：** 下面使用数学归纳法进行证明。     
当i=1时，第i层的节点数目为2<sup>i-1</sup>=2<sup>0</sup>=1。因为第1层上只有根节点，所以命题成立。 **（a）**    
假设当i>1，第i层的节点数目最多为2<sup>i-1</sup>个。由 **（a）** 推断得到。 **（b）**    
只需证明 **（b）** 中的假设即可证明原命题。要证明该假设，只需证明“第i+1层的节点数目最多为2<sup>i</sup>个”即可。     
由于二叉树的每个节点最多有两个孩子，故“第i+1层上的节点数目”最多是“第i层节点数目”的2倍。    
即第i+1层上的节点数目最多为：2\*2<sup>i-1</sup>=2<sup>i</sup>。
综上，性质1得证。    
##### 证明性质2
**证明：**    
当二叉树的每一层都含有最大节点数时，二叉树的节点数最多。由 **性质1** 得，深度为k的二叉树的节点数最多为：     
2<sup>0</sup>+2<sup>1</sup>+...+2<sup>k-1</sup> = 2<sup>k</sup>-1。原命题得证。
##### 证明性质3
**证明：**     
根据 **性质2** 可知，高度为h的二叉树最多有2<sup>h</sup>-1个节点，反之，对于包含n个节点的二叉树的高度至少为log<sub>2</sub>(n+1)。   
##### 证明性质4
**证明：**   
因为二叉树中所有任意节点的度数都不大于2，令n<sub>0</sub>为0度节点数，令n<sub>1</sub>为1度节点数，令n<sub>2</sub>为2度节点数，令n为总节点数。    
由此可得：n=n<sub>0</sub>+n<sub>1</sub>+n<sub>2</sub> **（a）**     
另外，0度结点没有孩子，1度结点有一个孩子，2度结点有两个孩子，故二叉树中孩子结点总数是：n<sub>1</sub>+2n<sub>2</sub> **（b）**    
结合 **（a）** 和 **（b）** 得n<sub>0</sub>=n<sub>2</sub>+1，得证。   
### 3 满二叉树，完全二叉树和二叉查找树
1. 满二叉树     
**定义：** 高度为h，并且有2<sup>h</sup>-1个节点的二叉树。
![image](https://user-images.githubusercontent.com/34436659/121886910-a2f16300-cd48-11eb-8b34-17402e00b889.png)
2. 完全二叉树    
**定义：** 一棵二叉树中，只有最下面两层结点的度可以小于2，并且最下一层的叶结点集中在靠左的若干位置上。这样的二叉树称为完全二叉树。     
**特点：** 叶子结点只能出现在最下层和次下层，且最下层的叶子结点集中在树的左部。显然，一棵满二叉树必定是一棵完全二叉树，而完全二叉树未必是满二叉树。     
![image](https://user-images.githubusercontent.com/34436659/121887236-1004f880-cd49-11eb-9f06-99d44c6bf4fb.png)     
3. 二叉查找树    
**定义：** 二叉查找树，又被称为二叉搜索树。设x为二叉查找树中的一个结点，x节点包含关键字key，节点x的key值记为key\[x\]。如果y是x的左子树中的一个结点，则key\[y\] ≤ key\[x\]；如果y是x的右子树的一个结点，则key\[y\] ≥ key\[x\]。     
![image](https://user-images.githubusercontent.com/34436659/121887562-79850700-cd49-11eb-8db9-0200b3cdd5e4.png)    
在二叉查找树中：
* 若任意节点的左子树不空，则左子树上所有结点的值均小于它的根结点的值；
* 任意节点的右子树不空，则右子树上所有结点的值均大于它的根结点的值；
* 任意节点的左、右子树也分别为二叉查找树；
* 没有键值相等的节点。

### 4 C++实现

参考文件MyBSTree.h。该文件实现了以下基本操作：
1. 插入节点；
2. 删除节点；
3. 前序遍历；
4. 中序遍历；
5. 后序遍历；
6. 返回二叉树查找树的最小值；
7. 返回二叉树查找树的最大值；
8. 查找键值为key的节点。