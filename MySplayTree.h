#pragma once
#include "MyStack.h"
#include <iostream>

using namespace std;

// �ڵ�
template <class _Ty>
struct MySplayTreeNode
{
    _Ty key;                    // ��ֵ
    MySplayTreeNode* left;      // ����
    MySplayTreeNode* right;     // �Һ���
    MySplayTreeNode* parent;    // ���ڵ�
    MySplayTreeNode() :left(nullptr), right(nullptr), parent(nullptr) {}
    MySplayTreeNode(_Ty val, MySplayTreeNode* l, MySplayTreeNode* r, MySplayTreeNode* p) : key(val), left(l), right(r) ,parent(p) {}
};

// ��չ����Splay Tree��
template <class _Ty>
class MySplayTree
{
public:
    MySplayTree() : m_root(nullptr), m_size(0) {}
    ~MySplayTree() {}

    // 1.�ȸ�����
    void preorder();            // �ݹ�汾
    void preorder_iterative();  // �����汾
    // 2.�и�����
    void inorder();             // �ݹ�汾
    void inorder_iterative();   // �����汾 
    // 3.�������
    void postorder();           // �ݹ�汾
    void postorder_iterative(); // �����汾
    // 4.����ڵ�
    void insert(_Ty key);
    // 5.ɾ���ڵ�
    void remove(_Ty key);
    // 6.���ҽڵ�
    bool contains(_Ty key);
    // 7.����������Сֵ
    _Ty minimum();
    // 8.�����������ֵ
    _Ty maximum();
    // 9.��ȡ���Ĵ�С
    size_t size();
    // 10.������չ��
    void destroy();
    
private:
    MySplayTreeNode<_Ty>* m_root;
    size_t m_size;
    // Ѱ�Ҳ���λ��
    MySplayTreeNode<_Ty>* search_pos(_Ty key);
    // Ѱ�Ҽ�ֵΪkey�Ľڵ�
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

    // ����ڵ�ǿջ���ջ�ǿգ�һֱִ��
    while (tmp || !stack.isEmpty())
    {
        // ��ǰ�ڵ�ǿգ�һֱ����������
        while (tmp)
        {
            stack.push(tmp);        // ��ǰ�ڵ�ѹջ
            stack.push(nullptr);    // ѹ��nullptr��
            tmp = tmp->left;        // ��������������
        }

        // �ߵ�����ڵ㣬���ջ�Ƿ���Ԫ��
        if (!stack.isEmpty())
        {
            tmp = stack.pop();             // ջ��Ԫ�س�ջ
            if (!tmp)                      // ���ջ����nullptr����ʾ��������û����
            {
                tmp = stack.top();         // ���ջ��Ԫ�أ�������ջ
                tmp = tmp->right;          // ����������
            }
            else                           // ջ��Ԫ�طǿգ���ʾ��������������
            {
                cout << tmp->key << " ";   // ����������
                tmp = nullptr;             // tmp��Ϊ�գ���Ϊ��һ��Ҫ�Ѹ��ڵ�ı�־��ջ
            }
        }
    }
}

template<class _Ty>
void MySplayTree<_Ty>::insert(_Ty key)
{
    // ���ڵ�Ϊ�գ�ֱ�Ӳ���
    if (!m_root)
    {
        m_root = new MySplayTreeNode<_Ty>(key, nullptr, nullptr, nullptr);
        m_size++;
        return;
    }
        
    // ���ڵ�ǿգ����ҵ�Ҫ�����λ��
    MySplayTreeNode<_Ty>* pos = search_pos(key);
    if (!pos)
        return;

    MySplayTreeNode<_Ty>* node = new MySplayTreeNode<_Ty>(key, nullptr, nullptr, pos);
    if (key < pos->key)
        pos->left = node;
    else
        pos->right = node;

    // ��ת�²���Ľڵ�Ϊ���ڵ�
    splay(node);
    m_size++;
}

template<class _Ty>
void MySplayTree<_Ty>::remove(_Ty key)
{
    MySplayTreeNode<_Ty>* tmp = search_node(key);
    if (tmp)
    {
        // ����key�ڵ㣬���ýڵ���ת������
        splay(tmp);
        // ���1��ֻ�и��ڵ�
        if (!tmp->left && !tmp->right)
        {
            m_root = nullptr;
        }
        // ���2��������Ϊ�գ�ֱ��ɾ�����ڵ㣬�Һ�����Ϊ�µ�����
        else if (!tmp->left)
        {
            m_root = tmp->right;
        }
        // ���3��������Ϊ�գ�ֱ��ɾ�����ڵ㣬������Ϊ����
        else if (!tmp->right)
        {
            m_root = tmp->left;
        }
        // ���4�����������ǿգ���������Ѱ��ǰ���ڵ㣨���ҽڵ㣩
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

    // �游�ڵ���ڵ�ʱ��һֱ����
    while (grandpa)
    {
        if (grandpa->left == parent && parent->left == node)     // ����
        {
            rotate_right(parent);
            rotate_right(node);
        }
        else if (grandpa->left == parent && parent->right == node)  // ����
        {
            rotate_left(node);
            rotate_right(node);
        }
        else if (grandpa->right == parent && parent->left == node)  // ����
        {
            rotate_right(node);
            rotate_left(node);
        }
        else if (grandpa->right == parent && parent->right == node)  // ����
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
        // �游�ڵ㲻���ڣ���תһ�μ������
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
