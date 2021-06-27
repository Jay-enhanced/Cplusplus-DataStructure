#pragma once

#include "MyException.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// AVL树节点
template <class _Ty>
class MyAVLTreeNode
{
public:
    _Ty key;
    int height;
    MyAVLTreeNode* left;
    MyAVLTreeNode* right;
    MyAVLTreeNode* parent;

    MyAVLTreeNode(_Ty key, MyAVLTreeNode* left, MyAVLTreeNode* right, MyAVLTreeNode* parent)
    {
        this->key = key;
        this->height = 1;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};

// AVL树
template <class _Ty>
class MyAVLTree
{
public:
	MyAVLTree();

    // 1.插入节点
    void insert(_Ty key);
    // 2.删除节点
    void remove(_Ty key);
    // 3.先根遍历
    void preOrder();
    // 4.中根遍历
    void inOrder();
    // 5.后根遍历
    void postOrder();
    // 6.获取树的高度
    int height();
    // 7.获取树的最大值
    _Ty max();
    // 8.获取树的最小值
    _Ty min();
    // 9.查找key是否在树中
    bool contains(_Ty key);
    // 10.销毁AVL树
    void destroy();
    // 11.打印AVL树
    void print();

private:
    MyAVLTreeNode<_Ty>* root;

    MyAVLTreeNode<_Ty>* search_insert(_Ty key);
    MyAVLTreeNode<_Ty>* search_remove(_Ty key);
    int getHeight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateLeft(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateRight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateLeftRight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateRightLeft(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* backTracking(MyAVLTreeNode<_Ty>* t);
    void destroy(MyAVLTreeNode<_Ty>* t);
    void preorder(MyAVLTreeNode<_Ty>* t);
    void inorder(MyAVLTreeNode<_Ty>* t);
    void postorder(MyAVLTreeNode<_Ty>* t);
    void print(MyAVLTreeNode<_Ty>* t, _Ty key, int direction);
};

template<class _Ty>
MyAVLTree<_Ty>::MyAVLTree()
{
    root = nullptr;
}

template<class _Ty>
void MyAVLTree<_Ty>::insert(_Ty key)
{
    MyAVLTreeNode<_Ty>* pos = search_insert(key);
    MyAVLTreeNode<_Ty>* tmp = new MyAVLTreeNode<_Ty>(key, nullptr, nullptr, nullptr);
    if (!tmp)
    {
        throw MyException("新建节点失败！\n");
    }

    // 根节点为空，直接插入
    if (!pos)
    {
        root = tmp;
        return;
    }

    if (key < pos->key) // 插入左边
    {
        pos->left = tmp;
    }
    else if (key > pos->key)
    {
        pos->right = tmp;
    }
    else
        return;

    tmp->parent = pos;
    pos->height = std::max(getHeight(pos->left), getHeight(pos->right)) + 1;
    root = backTracking(pos);   // 回溯
}

template<class _Ty>
void MyAVLTree<_Ty>::remove(_Ty key)
{
    // 先找到要删除的节点
    MyAVLTreeNode<_Ty>* pos = search_remove(key);
    if (!pos)
    {
        return;
    }

    // 如果是叶子节点，直接删除，并从父节点开始回溯
    if (!pos->left && !pos->right)
    {
        if (pos == root)
        {
            root->height = 0;
            delete root;
            root = nullptr;
            return;
        }

        MyAVLTreeNode<_Ty>* tmp = pos->parent;
        if (tmp->left == pos)    //该节点是父节点的左孩子
        {
            tmp->left = nullptr;
        }
        else
        {
            tmp->right = nullptr;
        }
        delete pos;
        pos = nullptr;
        tmp->height = std::max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
        root = backTracking(tmp);
        return;
    }

    // 被删除的节点是非叶子节点，需要从左右子树中找到键值仅次于key的节点作为晋升节点
    MyAVLTreeNode<_Ty>* _promotion_node = nullptr;
    if (pos->right)
    {
        // 右子树非空，从右子树找的左半边（包括子树的根）找晋升节点
        MyAVLTreeNode<_Ty>* _tmp = pos->right;
        while (1)
        {
            if (_tmp->left)
            {
                _tmp = _tmp->left;
            }
            else
            {
                _promotion_node = _tmp;
                break;
            }
        }
    }
    else
    {
        // 左子树非空，从左子树找的右半边（包括子树的根）找晋升节点
        MyAVLTreeNode<_Ty>* _tmp = pos->left;
        while (1)
        {
            if (_tmp->right)
            {
                _tmp = _tmp->right;
            }
            else
            {
                _promotion_node = _tmp;
                break;
            }
        }
    }

    // 晋升节点是从右子树找到的，有两种情况，一是叶子节点，二是只有右孩子
    // 晋升节点是从左子树找到的，有两种情况，一是叶子节点，二是只有左孩子
    MyAVLTreeNode<_Ty>* back_track = _promotion_node->parent;
    if (!_promotion_node->left && !_promotion_node->right)   // 叶子节点
    {
        // 是叶子节点，直接晋升
        if (_promotion_node->parent->left == _promotion_node)
            _promotion_node->parent->left = nullptr;
        else
            _promotion_node->parent->right = nullptr;
    }
    else if (!_promotion_node->left && _promotion_node->right)
    {
        _promotion_node->right->parent = _promotion_node->parent;
        _promotion_node->parent->right = _promotion_node->right;
    }
    else
    {
        _promotion_node->left->parent = _promotion_node->parent;
        _promotion_node->parent->left = _promotion_node->left;
    }

    pos->key = _promotion_node->key;
    delete _promotion_node;
    _promotion_node = nullptr;
    back_track->height = std::max(getHeight(back_track->left), getHeight(back_track->right)) + 1;
    root = backTracking(back_track);
}

template<class _Ty>
void MyAVLTree<_Ty>::preOrder()
{
    preorder(root);
}

template<class _Ty>
void MyAVLTree<_Ty>::inOrder()
{
    inorder(root);
}

template<class _Ty>
void MyAVLTree<_Ty>::postOrder()
{
    postorder(root);
}

template<class _Ty>
int MyAVLTree<_Ty>::height()
{
    if (!root)
        return 0;
    return root->height;
}

template<class _Ty>
_Ty MyAVLTree<_Ty>::max()
{
    MyAVLTreeNode<_Ty>* tmp = root;
    if (!tmp)
        throw MyException("这是一棵空树，无法找到最大值。\n");

    while (tmp)
    {
        if (tmp->right)
            tmp = tmp->right;
        else
            break;
    }

    return tmp->key;
}

template<class _Ty>
_Ty MyAVLTree<_Ty>::min()
{
    MyAVLTreeNode<_Ty>* tmp = root;
    if (!tmp)
        throw MyException("这是一棵空树，无法找到最大值。\n");

    while (tmp)
    {
        if (tmp->left)
            tmp = tmp->left;
        else
            break;
    }

    return tmp->key;
}

template<class _Ty>
bool MyAVLTree<_Ty>::contains(_Ty key)
{
    if (search_remove(key))
        return true;
    return false;
}

template<class _Ty>
void MyAVLTree<_Ty>::destroy()
{
    destroy(root);
    root = nullptr;
}

template<class _Ty>
void MyAVLTree<_Ty>::print()
{
    print(root, root->key, 0);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::search_insert(_Ty key)
{
    MyAVLTreeNode<_Ty>* tmp = root;
    while (tmp)
    {
        if (key < tmp->key)
        {
            if (!tmp->left)
                return tmp;
            tmp = tmp->left;
        }            
        else if (key > tmp->key)
        {
            if (!tmp->right)
                return tmp;
            tmp = tmp->right;
        }
        else if(key == tmp->key)
            throw MyException("树中已经存在该节点。\n");
    }

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::search_remove(_Ty key)
{
    MyAVLTreeNode<_Ty>* tmp = root;
    while (tmp)
    {
        if (tmp->key == key)
            return tmp;
        else if (key < tmp->key)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    return tmp;
}

template<class _Ty>
int MyAVLTree<_Ty>::getHeight(MyAVLTreeNode<_Ty>* t)
{
    if (!t)
        return 0;
    return t->height;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateLeft(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->right;
    tmp->parent = t->parent;
    if (t->parent)
    {
        t->parent->right = tmp;
    }
    
    t->right = tmp->left;
    t->parent = tmp;
    tmp->left = t;
    t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateRight(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->left;
    tmp->parent = t->parent;
    if (t->parent)
    {
        t->parent->left = tmp;
    }    
    t->left = tmp->right;
    t->parent = tmp;
    tmp->right = t;
    t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateLeftRight(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->left;
    tmp->left = tmp->right;
    tmp->right = nullptr;
    _Ty key = tmp->key;
    tmp->key = tmp->left->key;
    tmp->left->key = key;

    return rotateRight(t);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateRightLeft(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->right;
    tmp->right = tmp->left;
    tmp->left = nullptr;
    _Ty key = tmp->key;
    tmp->key = tmp->right->key;
    tmp->right->key = key;

    return rotateLeft(t);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::backTracking(MyAVLTreeNode<_Ty>* t)
{
    // 从t的父节点开始回溯
    MyAVLTreeNode<_Ty>* pback = t;
    MyAVLTreeNode<_Ty>* temp = pback->parent;
    while (temp != nullptr)
    {
        int lheight = getHeight(temp->left);
        int rheight = getHeight(temp->right);
        //temp->height = max(lheight, rheight) + 1;
        if ((lheight - rheight == 2) && temp->left->left)   // LL，右旋
        {
            temp = rotateRight(temp);
        }
        else if ((lheight - rheight == 2) && temp->left->right) // LR，先左旋再右旋
        {
            temp = rotateLeftRight(temp);
        }
        else if ((lheight - rheight == -2) && temp->right->right) // RR，左旋
        {
            temp = rotateLeft(temp);
        }
        else if ((lheight - rheight == -2) && temp->right->left) // RR，先右旋再左旋
        {
            temp = rotateRightLeft(temp);
        }
        pback = temp;
        temp = temp->parent;
    }
    pback->height = std::max(getHeight(pback->left), getHeight(pback->right)) + 1;
    return pback;
}

template<class _Ty>
void MyAVLTree<_Ty>::destroy(MyAVLTreeNode<_Ty>* t)
{
    if (!t)
        return;

    if(t->left)
        destroy(t->left);
    if (t->right)
        destroy(t->right);

    delete t;
}

template<class _Ty>
void MyAVLTree<_Ty>::preorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        cout << t->key << " ";
        preorder(t->left);
        preorder(t->right);
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::inorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);
        cout << t->key << " ";
        inorder(t->right);
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::postorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);        
        inorder(t->right);
        cout << t->key << " ";
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::print(MyAVLTreeNode<_Ty>* t, _Ty key, int direction)
{
    if (t)
    {
        if (direction == 0)
            cout << setw(2) << t->key << "is root." << endl;
        else
            cout << setw(2) << t->key << "is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child." : "left child.") << endl;

        print(t->left, t->key, -1);
        print(t->right, t->key, 1);
    }
}
