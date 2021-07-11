#pragma once
#include <iostream>

using namespace std;
// 节点
template <class _Ty>
class MyRBTreeNode
{
public:
    enum class RBTreeColor { RED, BLACK };    // 红黑树的颜色

    _Ty key;                        // 键值
    RBTreeColor color   = RBTreeColor::RED;      // 颜色
    MyRBTreeNode* left  = nullptr;  // 左孩子
    MyRBTreeNode* right = nullptr;  // 右孩子
    MyRBTreeNode* parent;           // 父节点

    MyRBTreeNode(_Ty val, MyRBTreeNode* p) :
        key(val), parent(p) {}
};

// 红黑树
template <class _Ty>
class MyRBTree
{
public:
    MyRBTree() : root(nullptr) {}     // 构造函数
    ~MyRBTree() {}    // 析构函数

    // 1.插入节点
    void insert(_Ty key);
    // 中序遍历
    void inorder();
    void inorder_iterative();
    
private:
    MyRBTreeNode<_Ty> *root;

    // 判断是否需要修正
    bool needed_fixup(MyRBTreeNode<_Ty>* node);
    // 插入修正
    void insert_fixup(MyRBTreeNode<_Ty>* node);
    // 左旋
    MyRBTreeNode<_Ty>* rotate_left(MyRBTreeNode<_Ty>* node);
    // 左旋
    MyRBTreeNode<_Ty>* rotate_right(MyRBTreeNode<_Ty>* node);
    // 设置黑色
    void set_black(MyRBTreeNode<_Ty>* node);
    // 设置红色
    void set_red(MyRBTreeNode<_Ty>* node);
    // 中序遍历，内部接口，递归版本
    void inorder(MyRBTreeNode<_Ty>* t);
};

template<class _Ty>
void MyRBTree<_Ty>::insert(_Ty key)
{
    MyRBTreeNode<_Ty>* tmp = root;
    MyRBTreeNode<_Ty>* parent = nullptr;

    // 找到插入位置
    while (tmp)
    {
        parent = tmp;
        if (key < tmp->key)
            tmp = tmp->left;
        else if (key > tmp->key)
            tmp = tmp->right;
        else return;
    }

    // 新建节点
    MyRBTreeNode<_Ty>* node = new MyRBTreeNode<_Ty>(key, parent);
    if (!node)
        return;

    // 父节点非空，将新节点设置为父节点的左孩子或右孩子
    if (parent)
    {
        if (node->key < parent->key)
            parent->left = node;
        else
            parent->right = node;
    }
    // 父节点是空的，新节点作为根节点
    else
        root = node;

    // 将树修正为红黑树
    insert_fixup(node);
}

template<class _Ty>
void MyRBTree<_Ty>::inorder()
{
    inorder(root);
}

template<class _Ty>
bool MyRBTree<_Ty>::needed_fixup(MyRBTreeNode<_Ty>* node)
{
    if (node->parent && node->parent->parent && node->parent->color == MyRBTreeNode<_Ty>::RBTreeColor::RED)
        return true;
    return false;
}

template<class _Ty>
void MyRBTree<_Ty>::insert_fixup(MyRBTreeNode<_Ty>* node)
{
    MyRBTreeNode<_Ty>* parent  = nullptr;
    MyRBTreeNode<_Ty>* grandpa = nullptr;
    MyRBTreeNode<_Ty>* uncle = nullptr;
    
    // 判断是否需要调整。只有祖父节点存在，并且父节点是红色的，才需要调整
    while (needed_fixup(node))
    {
        parent = node->parent;
        grandpa = parent->parent;

        // 情况1：左左
        if (node == parent->left && parent == grandpa->left)
        {
            uncle = grandpa->right;
            // (1) 叔叔黑。爹变黑，爷变红，右旋一次完成调整。
            if (!uncle || uncle->color == MyRBTreeNode<_Ty>::RBTreeColor::BLACK)
            {
                MyRBTreeNode<_Ty>* tmp;
                tmp = rotate_right(parent);
                if (!tmp->parent)
                    root = tmp;
                set_black(parent);
                set_red(grandpa);
                break;
            }
            // (2) 叔叔红。爹和叔叔变黑，爷爷变红，爷爷作为当前节点，回溯
            else
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
        }
        // 情况2：左右，左旋当前节点，然后将旋转后的左孩子当作当前节点，变成情况1
        else if (node == parent->right && parent == grandpa->left)
        {
            uncle = grandpa->right;
            if (uncle && uncle->color == MyRBTreeNode<_Ty>::RBTreeColor::RED)
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
            else
            {
                rotate_left(node);
                node = node->left;
            }
        }
        // 情况3：右右。
        else if (node == parent->right && parent == grandpa->right)
        {
            uncle = grandpa->left;
            // （1）叔叔黑。爹变黑，爷变红，左旋父节点一次完成调整。
            if (!uncle || uncle->color == MyRBTreeNode<_Ty>::RBTreeColor::BLACK)
            {
                MyRBTreeNode<_Ty>* tmp;
                tmp = rotate_left(parent);
                if (!tmp->parent)
                    root = tmp;
                set_black(parent);
                set_red(grandpa);
                break;
            }
            // （2）叔叔红。爹和叔叔变黑，爷爷变红，爷爷作为当前节点，回溯
            else
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
        }
        // 情况4：右左，右旋当前节点，然后将旋转后的右孩子当作当前节点，变成情况3。
        else if (node == parent->left && parent == grandpa->right)
        {
            uncle = grandpa->left;
            if (uncle && uncle->color == MyRBTreeNode<_Ty>::RBTreeColor::RED)
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
            else
            {
                rotate_right(node);
                node = node->right;
            }
        }
    }

    // 根节点设为黑色
    set_black(root);
}

template<class _Ty>
MyRBTreeNode<_Ty>* MyRBTree<_Ty>::rotate_left(MyRBTreeNode<_Ty>* node)
{
    MyRBTreeNode<_Ty>* parent = node->parent;
    MyRBTreeNode<_Ty>* grandpa = parent->parent;

    node->parent = grandpa;         // 爷爷变爹
    parent->right = node->left;     // 自己的左孩子送给爹做右孩子
    node->left = parent;            // 把爹当作自己的左孩子
    parent->parent = node;          // 爹认儿子做爹

    if (grandpa)
    {
        if(grandpa->left == parent)
            grandpa->left = node;
        else
            grandpa->right = node;
    }

    if (parent->right)
        parent->right->parent = parent;

    return node;
}

template<class _Ty>
MyRBTreeNode<_Ty>* MyRBTree<_Ty>::rotate_right(MyRBTreeNode<_Ty>* node)
{
    MyRBTreeNode<_Ty>* parent = node->parent;
    MyRBTreeNode<_Ty>* grandpa = parent->parent;

    node->parent = grandpa;         // 爷爷变爹
    parent->left = node->right;     // 自己的右孩子送给爹做左孩子
    node->right = parent;           // 把爹当作自己的右孩子
    parent->parent = node;          // 爹认儿子做爹

    if (grandpa)
    {
        if (grandpa->left == parent)
            grandpa->left = node;
        else
            grandpa->right = node;
    }

    if (parent->left)
        parent->left->parent = parent;

    return node;
}

template<class _Ty>
inline void MyRBTree<_Ty>::set_black(MyRBTreeNode<_Ty>* node)
{
    if (node)
        node->color = MyRBTreeNode<_Ty>::RBTreeColor::BLACK;
}

template<class _Ty>
inline void MyRBTree<_Ty>::set_red(MyRBTreeNode<_Ty>* node)
{
    if (node)
        node->color = MyRBTreeNode<_Ty>::RBTreeColor::RED;
}

template<class _Ty>
void MyRBTree<_Ty>::inorder(MyRBTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);
        cout << t->key << "(" << (t->color == MyRBTreeNode<_Ty>::RBTreeColor::RED ? "RED" : "BLACK") << ")" << ":";
        cout << "left->";
        if (t->left)
            cout << t->left->key << ", ";
        else
            cout << "NULL, ";
        cout << "right->";
        if (t->right)
            cout << t->right->key;
        else
            cout << "NULL";
        cout << endl;
        inorder(t->right);
    }
}
