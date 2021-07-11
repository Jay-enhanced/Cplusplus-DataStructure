#pragma once

// �ڵ�
template <class _Ty>
class MyRBTreeNode
{
public:
    enum class RBTreeColor { RED, BLACK };    // ���������ɫ

    _Ty key;                        // ��ֵ
    RBTreeColor color   = RBTreeColor::RED;      // ��ɫ
    MyRBTreeNode* left  = nullptr;  // ����
    MyRBTreeNode* right = nullptr;  // �Һ���
    MyRBTreeNode* parent;           // ���ڵ�

    MyRBTreeNode(_Ty val, MyRBTreeNode* p) :
        key(val), parent(p) {}
};

// �����
template <class _Ty>
class MyRBTree
{
public:
    MyRBTree() : root(nullptr) {}     // ���캯��
    ~MyRBTree() {}    // ��������

    // 1.����ڵ�
    void insert(_Ty key);
    
private:
    MyRBTreeNode<_Ty> *root;

    // �ж��Ƿ���Ҫ����
    bool needed_fixup(MyRBTreeNode<_Ty>* node);
    // ��������
    void insert_fixup(MyRBTreeNode<_Ty>* node);
    // ����
    MyRBTreeNode<_Ty>* rotate_left(MyRBTreeNode<_Ty>* node);
    // ����
    MyRBTreeNode<_Ty>* rotate_right(MyRBTreeNode<_Ty>* node);
    // ���ú�ɫ
    void set_black(MyRBTreeNode<_Ty>* node);
    // ���ú�ɫ
    void set_red(MyRBTreeNode<_Ty>* node);
};

template<class _Ty>
void MyRBTree<_Ty>::insert(_Ty key)
{
    MyRBTreeNode<_Ty>* tmp = root;
    MyRBTreeNode<_Ty>* parent = nullptr;

    // �ҵ�����λ��
    while (tmp)
    {
        parent = tmp;
        if (key < tmp->key)
            tmp = tmp->left;
        else if (key > tmp->key)
            tmp = tmp->right;
        else return;
    }

    // �½��ڵ�
    MyRBTreeNode<_Ty>* node = new MyRBTreeNode<_Ty>(key, parent);
    if (!node)
        return;

    // ���ڵ�ǿգ����½ڵ�����Ϊ���ڵ�����ӻ��Һ���
    if (parent)
    {
        if (node->key < parent->key)
            parent->left = node;
        else
            parent->right = node;
    }
    // ���ڵ��ǿյģ��½ڵ���Ϊ���ڵ�
    else
        root = node;

    // ��������Ϊ�����
    insert_fixup(node);
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
    // ����游�ڵ���ڣ�����Ҫ����
    while (needed_fixup(node))
    {
        parent = node->parent;
        grandpa = parent->parent;

        // ���1������
        if (node == parent->left && parent == grandpa->left)
        {
            uncle = grandpa->right;
            // ����ڡ�����ڣ�ү��죬����һ����ɵ�����
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
            // ����졣���������ڣ�үү��죬үү��Ϊ��ǰ�ڵ㣬����
            else
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
        }
        // ���2�����ң�������������1
        else if (node == parent->right && parent == grandpa->left)
        {
            rotate_left(node);
            node = node->left;
        }
        // ���3�����ҡ�
        else if (node == parent->right && parent == grandpa->right)
        {
            uncle = grandpa->left;
            // ����ڡ�����ڣ�ү��죬����һ����ɵ�����
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
            // ����졣���������ڣ�үү��죬үү��Ϊ��ǰ�ڵ㣬����
            else
            {
                set_black(parent);
                set_black(uncle);
                set_red(grandpa);
                node = grandpa;
            }
        }
        // ���4������
        else if (node == parent->left && parent == grandpa->right)
        {
            rotate_right(node);
            node = node->right;
        }
    }

    // ���ڵ���Ϊ��ɫ
    set_black(root);
}

template<class _Ty>
MyRBTreeNode<_Ty>* MyRBTree<_Ty>::rotate_left(MyRBTreeNode<_Ty>* node)
{
    MyRBTreeNode<_Ty>* parent = node->parent;
    MyRBTreeNode<_Ty>* grandpa = parent->parent;

    node->parent = grandpa;         // үү���
    parent->right = node->left;     // �Լ��������͸������Һ���
    node->left = parent;            // �ѵ������Լ�������
    parent->parent = node;          // ���϶�������

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

    node->parent = grandpa;         // үү���
    parent->left = node->right;     // �Լ����Һ����͸���������
    node->right = parent;           // �ѵ������Լ����Һ���
    parent->parent = node;          // ���϶�������

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
