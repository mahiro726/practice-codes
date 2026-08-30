#include <iostream>

namespace lcc
{
    template <class K, class V>
    struct AVLTNode
    {
        AVLTNode(const pair<K, V> &kv = pair<K, V>())
            : _kv(kv),
              _left(nullptr),
              _right(nullptr),
              _parent(nullptr),
              _bf(0)
        {
        }

        pair<K, V> _kv;
        AVLTNode<K, V> *_left;
        AVLTNode<K, V> *_right;
        AVLTNode<K, V> *_parent;
        int _bf;
    };

    template <class K, class V>
    class AVLTree
    {
        typedef AVLTNode<K, V> Node;

    private:
        void _Inorder(Node *root)
        {
            if (root == nullptr)
            {
                return;
            }

            _Inorder(root->_left);
            std::cout << root->_kv.first << ":" << root->_kv.second << std::endl;
            _Inorder(root->_right);
        }

    public:
        AVLTree()
            : _root(nullptr)
        {
        }

        bool Insert(const pair<K, V> &kv)
        {
            if (_root == nullptr)
            {
                _root = new Node(kv);
                return true;
            }

            // 先比大小
            Node *parent = nullptr;
            Node *cur = _root;
            while (cur)
            {
                parent = cur;
                if (kv.first < cur->_kv.first)
                {
                    cur = cur->_left;
                }
                else if (kv.first > cur->_kv.first)
                {
                    cur = cur->_right;
                }
                else
                {
                    return false;
                }
            }

            cur = new Node(kv);
            cur->_bf = 0;
            if (kv.first < parent->_kv.first)
            {
                parent->_left = cur;
            }
            else if (kv.first > parent->_kv.first)
            {
                parent->_right = cur;
            }
            cur->_parent = parent;

            // 第二步：这里接 BF 更新循环
            while (parent)
            {
                if (cur == parent->_left)
                {
                    parent->_bf--;
                }
                else
                {
                    parent->_bf++;
                }

                if (parent->_bf == 0)
                {
                    break;
                }
                else if (parent->_bf == 1 || parent->_bf == -1)
                {
                    // 继续
                    cur = parent;
                    parent = parent->_parent;
                }
                else if (parent->_bf == 2 || parent->_bf == -2)
                {
                    // 触发右旋转
                    if (parent->_bf == -2 && cur->_bf == -1)
                    {
                        RotateR(parent);
                    }
                    break;
                }
                else
                {
                    // 直接报错，说明之前就是错的
                    assert(false);
                }
            }
            return true;
        }

        // 右单旋函数
        void RotateR(Node *parent)
        {
            Node *subL = parent->_left;
            Node *subLR = subL->_right;

            parent->_left = subLR;
            if (subLR)
                subLR->_parent = parent;

            Node *parentParent = parent->_parent;

            subL->_right = parent;
            parent->_parent = subL;

            if (parent == _root)
            {
                _root = subL;
                subL->_parent = nullptr;
            }
            else
            {
                if (parentParent->_left == parent)
                {
                    parentParent->_left = subL;
                    subL->_parent = parentParent;
                }
                else
                {
                    parentParent->_right = subL;
                    subL->_parent = parentParent;
                }
            }

            // 更新 subL 和 parent 的 _bf
            subL->_bf = 0;
            parent->_bf = 0;
        }

        ~AVLTree() {}

    private:
        Node *_root;
    };
}