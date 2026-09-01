#include <iostream>
#include <utility>

namespace lcc
{
    enum Color
    {
        RED,
        BLACK
    };

    template <class K, class V>
    struct RBTNode
    {
        pair<K, V> _kv;

        RBTNode<K, V> *_left;
        RBTNode<K, V> *_right;
        RBTNode<K, V> *_parent;

        Color _color;

        RBTNode(const pair<K, V> &kv)
            : _kv(kv),
              _left(nullptr),
              _right(nullptr),
              _parent(nullptr),
              _color(RED)
        {
        }
    };

    template <class K, class V>
    class RBTree
    {
        typedef RBTNode<K, V> Node;

    public:
        RBTree()
            : _root(nullptr)
        {
        }

        bool Insert(const pair<K, V> &kv)
        {
            if (_root == nullptr)
            {
                _root = new Node(kv);
                _root->_color = BLACK;
                return true;
            }

            // 先找到插入的位置
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

            // 找到位置了，进行插入
            cur = new Node(kv);
            if (kv.first < parent->_kv.first)
            {
                parent->_left = cur;
            }
            else if (kv.first > parent->_kv.first)
            {
                parent->_right = cur;
            }
            cur->_parent = parent;

            //---------------------------------------------------变色 + 旋转-----------------------------------------------------

            while (parent && parent->_color == RED)
            {
                Node *grandparent = parent->_parent;

                if (grandparent == nullptr) // parent为红时，grandparent必存在
                {
                    break;
                }

                if (grandparent->_left == parent)
                {
                    Node *uncle = grandparent->_right;
                    // 叔叔存在且为红 -> 变色
                    if (uncle && uncle->_color == RED)
                    {
                        parent->_color = BLACK;
                        uncle->_color = BLACK;
                        grandparent->_color = RED;

                        // 继续往上处理
                        cur = grandparent;
                        parent = cur->_parent;
                    }
                    else // 叔叔不存在，或者叔叔存在且为黑 -> 旋转+变色
                    {
                        if (cur == parent->_left)
                        {
                            //      g
                            //   p     u
                            // c
                            // 右单旋
                            RotateR(grandparent);
                            parent->_color = BLACK;
                            grandparent->_color = RED;
                        }
                        else
                        {
                            //     g
                            //  p     u
                            //    c
                            RotateL(parent);
                            RotateR(grandparent);
                            // 变色
                            cur->_color = BLACK;
                            grandparent = RED;
                        }
                        break;
                    }
                }
                else // 爷爷节点的右边是父节点
                {
                    Node *uncle = grandparent->_left;
                    // 叔叔存在且为红 -> 变色
                    if (uncle && uncle->_color == RED)
                    {
                        parent->_color = BLACK;
                        uncle->_color = BLACK;
                        grandparent->_color = RED;

                        // 继续往上处理
                        cur = grandparent;
                        parent = cur->_parent;
                    }
                    else // 叔叔不存在，或者叔叔存在且为黑 -> 旋转+变色
                    {
                        if (cur == parent->_left)
                        {
                            //      g
                            //   u     p
                            //            c
                            // 左单旋
                            RotateL(grandparent);
                            parent->_color = BLACK;
                            grandparent->_color = RED;
                        }
                        else
                        {
                            //     g
                            //  u     p
                            //      c
                            RotateR(parent);
                            RotateL(grandparent);
                            // 变色
                            cur->_color = BLACK;
                            grandparent->_color = RED;
                        }
                        break;
                    }
                }
            }

            _root->_color = BLACK;

            return true;
        }

        // 右单旋
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
        }

        // 左单旋
        void RotateL(Node *parent)
        {
            Node *subR = parent->_right;
            Node *subRL = subR->_left;

            parent->_right = subRL;
            if (subRL)
                subRL->_parent = parent;

            Node *parentParent = parent->_parent;

            subR->_left = parent;
            parent->_parent = subR;

            if (parent == _root)
            {
                _root = subR;
                subR->_parent = nullptr;
            }
            else
            {
                if (parentParent->_left == parent)
                {
                    parentParent->_left = subR;
                    subR->_parent = parentParent;
                }
                else
                {
                    parentParent->_right = subR;
                    subR->_parent = parentParent;
                }
            }
        }

        void _inorder(Node *root)
        {
            if (root == nullptr)
                return;
            _inorder(root->_left);
            std::cout << root->_kv.first << " ";
            _inorder(root->_right);
        }

        int _check(Node *root) // 违规返回 -1，合法返回黑高
        {
            if (root == nullptr)
                return 0;
            if (root->_color == RED &&
                ((root->_left && root->_left->_color == RED) ||
                 (root->_right && root->_right->_color == RED)))
                return -1; // 红不相邻
            int l = _check(root->_left), r = _check(root->_right);
            if (l == -1 || r == -1 || l != r)
                return -1; // 黑高相等
            return root->_color == BLACK ? l + 1 : l;
        }

        void Inorder()
        {
            _inorder(_root);
            std::cout << std::endl;
        }
        bool IsValidRBTree() { return _root && _root->_color == BLACK && _check(_root) != -1; }

        ~RBTree()
        {
        }

    private:
        Node *_root;
    };
}