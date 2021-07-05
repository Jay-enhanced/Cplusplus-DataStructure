#pragma once
#include "MyStack.h"
#include <iostream>

using namespace std;

// 节点
template <class _Ty>
struct MySplayTreeNode
{
    _Ty key;                    // 键值
    MySplayTreeNode* left;      // 左孩子
    MySplayTreeNode* right;     // 右孩子
    MySplayTreeNode* parent;    // 父节点
    MySplayTreeNode() :left(nullptr), right(nullptr), parent(nullptr) {}
    MySplayTreeNode(_Ty val, MySplayTreeNode* l, MySplayTreeNode* r, MySplayTreeNode* p) : key(val), left(l), right(r) ,parent(p) {}
};

// 伸展树（Splay Tree）
template <class _Ty>
class MySplayTree
{
public:
    MySplayTree() : m_root(nullptr), m_size(0) {}
    ~MySplayTree() {}

    // 1.先根遍历
    void preorder();            // 递归版本
    void preorder_iterative();  // 迭代版本
    // 2.中根遍历
    void inorder();             // 递归版本
    void inorder_iterative();   // 迭代版本 
    // 3.后根遍历
    void postorder();           // 递归版本
    void postorder_iterative(); // 迭代版本
    // 4.插入节点
    void insert(_Ty key);
    // 5.删除节点
    void remove(_Ty key);
    // 6.查找节点
    bool contains(_Ty key);
    // 7.查找树中最小值
    _Ty minimum();
    // 8.查找树中最大值
    _Ty maximum();
    // 9.获取树的大小
    size_t size();
    // 10.销毁伸展树
    void destroy();
    
private:
    MySplayTreeNode<_Ty>* m_root;
    size_t m_size;
    // 寻找插入位置
    MySplayTreeNode<_Ty>* search_pos(_Ty key);
    // 寻找键值为key的节点
    MySplayTreeNode<_Ty>* search_node(_Ty key);
    void splay(MySplayTreeNode<_Ty>* node);
    void rotate_right(MySplayTreeNode<_Ty>* node);
    void rotate_left(MySplayTreeNode<_Ty>* node);
    void inorder(MySplayTreeNode<_Ty>* t);
    void preorder(MySplayTreeNode<_Ty>* t);
    void postorder(MySplayTreeNode<_Ty>* t);
};

template<class _Ty>
void MySplayTree<_Ty>::preorder()
{
    preorder(m_root);
}

template<class _Ty>
void MySplayTree<_Ty>::preorder_iterative()
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    MyStack<MySplayTreeNode<_Ty>*> stack(m_size);

    while (tmp || !stack.isEmpty())
    {
        while (tmp)
        {
            cout << tmp->key << " ";
            stack.push(tmp);
            tmp = tmp->left;
        }

        if (!stack.isEmpty())
        {
            tmp = stack.pop();
            tmp = tmp->right;
        }
    }
}

template<class _Ty>
void MySplayTree<_Ty>::inorder()
{
    inorder(m_root);
}

template<class _Ty>
void MySplayTree<_Ty>::inorder_iterative()
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    MyStack<MySplayTreeNode<_Ty>*> stack(m_size);

    while (tmp || !stack.isEmpty())
    {
        while (tmp)
        {
            stack.push(tmp);
            tmp = tmp->left;
        }

        if (!stack.isEmpty())
        {
            tmp = stack.pop();
            cout << tmp->key << " ";
            tmp = tmp->right;
        }
    }
}

template<class _Ty>
void MySplayTree<_Ty>::postorder()
{
    postorder(m_root);
}

template<class _Ty>
void MySplayTree<_Ty>::postorder_iterative()
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    MyStack<MySplayTreeNode<_Ty>*> stack(2 * m_size);

    // 如果节点非空或者栈非空，一直执行
    while (tmp || !stack.isEmpty())
    {
        // 当前节点非空，一直往左子树走
        while (tmp)
        {
            stack.push(tmp);        // 当前节点压栈
            stack.push(nullptr);    // 压入nullptr作
            tmp = tmp->left;        // 继续往左子树走
        }

        // 走到最左节点，检查栈是否有元素
        if (!stack.isEmpty())
        {
            tmp = stack.pop();             // 栈顶元素出栈
            if (!tmp)                      // 如果栈顶是nullptr，表示右子树还没遍历
            {
                tmp = stack.top();         // 获得栈顶元素，但不出栈
                tmp = tmp->right;          // 往右子树走
            }
            else                           // 栈顶元素非空，表示右子树遍历完了
            {
                cout << tmp->key << " ";   // 访问子树根
                tmp = nullptr;             // tmp置为空，因为下一步要把父节点的标志出栈
            }
        }
    }
}

template<class _Ty>
void MySplayTree<_Ty>::insert(_Ty key)
{
    // 根节点为空，直接插入
    if (!m_root)
    {
        m_root = new MySplayTreeNode<_Ty>(key, nullptr, nullptr, nullptr);
        m_size++;
        return;
    }
        
    // 根节点非空，先找到要插入的位置
    MySplayTreeNode<_Ty>* pos = search_pos(key);
    if (!pos)
        return;

    MySplayTreeNode<_Ty>* node = new MySplayTreeNode<_Ty>(key, nullptr, nullptr, pos);
    if (key < pos->key)
        pos->left = node;
    else
        pos->right = node;

    // 旋转新插入的节点为根节点
    splay(node);
    m_size++;
}

template<class _Ty>
void MySplayTree<_Ty>::remove(_Ty key)
{
    MySplayTreeNode<_Ty>* tmp = search_node(key);
    if (tmp)
    {
        // 存在key节点，将该节点旋转至树根
        splay(tmp);
        // 情况1：只有根节点
        if (!tmp->left && !tmp->right)
        {
            m_root = nullptr;
        }
        // 情况2：左子树为空，直接删除根节点，右孩子作为新的树根
        else if (!tmp->left)
        {
            m_root = tmp->right;
        }
        // 情况3：右子树为空，直接删除根节点，左孩子作为树根
        else if (!tmp->right)
        {
            m_root = tmp->left;
        }
        // 情况4：左右子树非空，从左子树寻找前驱节点（最右节点）
        else
        {
            tmp = tmp->left;
            while (tmp)
            {
                if (tmp->right)
                    tmp = tmp->right;
                else
                    break;
            }
            m_root->key = tmp->key;
            tmp->parent->right = tmp->left;
            if(tmp->left)
                tmp->left->parent = tmp->parent;
            tmp->parent = nullptr;
            tmp->left = nullptr;
        }

        delete tmp;
        tmp = nullptr;
        m_size--;
    }
}

template<class _Ty>
bool MySplayTree<_Ty>::contains(_Ty key)
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    while (tmp)
    {
        if (key < tmp->key)
            tmp = tmp->left;
        else if (key > tmp->key)
            tmp = tmp->right;
        else
        {
            splay(tmp);
            return true;
        }
    }
    return false;
}

template<class _Ty>
_Ty MySplayTree<_Ty>::minimum()
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    while (tmp)
    {
        if (tmp->left)
            tmp = tmp->left;
        else
            return tmp->key;
    }
    throw MyException("This Splay tree is empty.\n");
}

template<class _Ty>
_Ty MySplayTree<_Ty>::maximum()
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    while (tmp)
    {
        if (tmp->right)
            tmp = tmp->right;
        else
            return tmp->key;
    }
    throw MyException("This Splay tree is empty.\n");
}

template<class _Ty>
inline size_t MySplayTree<_Ty>::size()
{
    return m_size;
}

template<class _Ty>
void MySplayTree<_Ty>::destroy()
{

}

template<class _Ty>
MySplayTreeNode<_Ty>* MySplayTree<_Ty>::search_pos(_Ty key)
{
    MySplayTreeNode<_Ty>* pos = nullptr;
    MySplayTreeNode<_Ty>* tmp = m_root;

    while (tmp)
    {
        pos = tmp;
        if (key < tmp->key)
        {
            tmp = tmp->left;
        }
        else if (key > tmp->key)
        {
            tmp = tmp->right;
        }
        else
            return nullptr;
    }

    return pos;
}

template<class _Ty>
MySplayTreeNode<_Ty>* MySplayTree<_Ty>::search_node(_Ty key)
{
    MySplayTreeNode<_Ty>* tmp = m_root;
    while (tmp)
    {
        if (key < tmp->key)
            tmp = tmp->left;
        else if (key > tmp->key)
            tmp = tmp->right;
        else
            return tmp;
    }
    return nullptr;
}

template<class _Ty>
void MySplayTree<_Ty>::splay(MySplayTreeNode<_Ty>* node)
{
    if (node == m_root)
        return;

    MySplayTreeNode<_Ty>* grandpa = nullptr;
    MySplayTreeNode<_Ty>* parent = node->parent;
    if(parent)
        grandpa = node->parent->parent;

    // 祖父节点存在的时候，一直回溯
    while (grandpa)
    {
        if (grandpa->left == parent && parent->left == node)     // 左左
        {
            rotate_right(parent);
            rotate_right(node);
        }
        else if (grandpa->left == parent && parent->right == node)  // 左右
        {
            rotate_left(node);
            rotate_right(node);
        }
        else if (grandpa->right == parent && parent->left == node)  // 右左
        {
            rotate_right(node);
            rotate_left(node);
        }
        else if (grandpa->right == parent && parent->right == node)  // 右右
        {
            rotate_left(parent);
            rotate_left(node);
        }
        parent = node->parent;
        if (parent)
            grandpa = parent->parent;
        else
            grandpa = nullptr;
    }

    if (!grandpa && parent)
    {
        // 祖父节点不存在，旋转一次即可完成
        if (parent->left == node)
            rotate_right(node);
        else
            rotate_left(node);
    }

    m_root = node;
}

template<class _Ty>
void MySplayTree<_Ty>::rotate_right(MySplayTreeNode<_Ty>* node)
{
    MySplayTreeNode<_Ty>* tmp = node->parent;
    tmp->left = node->right;
    if(tmp->left)
        tmp->left->parent = tmp;
    node->right = tmp;
    node->parent = tmp->parent;
    if (node->parent)
    {
        if (node->parent->right == tmp)
            node->parent->right = node;
        else
            node->parent->left = node;
    }
    tmp->parent = node;
}

template<class _Ty>
void MySplayTree<_Ty>::rotate_left(MySplayTreeNode<_Ty>* node)
{
    MySplayTreeNode<_Ty>* tmp = node->parent;
    tmp->right = node->left;
    if(tmp->right)
        tmp->right->parent = tmp;
    node->left = tmp;
    node->parent = tmp->parent;
    if (node->parent)
    {
        if (node->parent->right == tmp)
            node->parent->right = node;
        else
            node->parent->left = node;
    }
    tmp->parent = node;
}

template<class _Ty>
void MySplayTree<_Ty>::inorder(MySplayTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);
        cout << t->key << " ";
        //cout << "key: " << t->key << endl;
        //if(t->left)
        //    cout << "left: " << t->left->key << endl;
        //if(t->right)
        //    cout << "right: " << t->right->key << endl;
        inorder(t->right);
    }
}

template<class _Ty>
void MySplayTree<_Ty>::preorder(MySplayTreeNode<_Ty>* t)
{
    if (t)
    {
        cout << t->key << " ";
        preorder(t->left);
        preorder(t->right);
    }
}

template<class _Ty>
void MySplayTree<_Ty>::postorder(MySplayTreeNode<_Ty>* t)
{
    if (t)
    {
        postorder(t->left);
        postorder(t->right);
        cout << t->key << " ";
    }
}
