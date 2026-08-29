#include <iostream>

namespace lcc
{
    template <class T>
    struct BSTNode
    {
    public:
        BSTNode(const T &key)
        {
            _key = key;
            _right = nullptr;
            _left = nullptr;
        }

        ~BSTNode() {}

        T _key;
        BSTNode<T> *_right;
        BSTNode<T> *_left;
    };

    template <class T>
    class BSTree
    {
        typedef BSTNode<T> Node;

    private:
        void _inorder(Node *root)
        {
            Node *cur = root;

            if (cur == nullptr)
            {
                return;
            }

            _inorder(cur->_left);
            std::cout << cur->_key << " ";
            _inorder(cur->_right);
        }

    public:
        BSTree()
        {
            _root = nullptr;
        }

        // 循环版
        bool Insert(const T &key)
        {
            if (_root == nullptr)
            {
                _root = new Node(key);
                return true;
            }

            // 先比大小
            Node *parent = nullptr;
            Node *cur = _root;
            while (cur)
            {
                parent = cur;
                if (key < cur->_key)
                {
                    cur = cur->_left;
                }
                else if (key > cur->_key)
                {
                    cur = cur->_right;
                }
                else
                {
                    return false;
                }
            }

            if (key < parent->_key)
            {
                parent->_left = new Node(key);
            }
            else if (key > parent->_key)
            {
                parent->_right = new Node(key);
            }
            return true;
        }

        bool Find(const T &key)
        {
            Node *cur = _root;

            while (cur)
            {
                if (key < cur->_key)
                {
                    cur = cur->_left;
                }
                else if (key > cur->_key)
                {
                    cur = cur->_right;
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        bool Erase(const T &key)
        {
            Node *parent = nullptr;
            Node *cur = _root;

            while (cur)
            {
                if (key < cur->_key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (key > cur->_key)
                {
                    parent = cur; // parent只在cur下走时更新
                    cur = cur->_right;
                }
                else
                {
                    // 准备删除
                    // 左为空
                    if (cur->_left == nullptr)
                    {
                        if (parent->_left == cur)
                        {
                            parent->_left = cur->_right;
                        }
                        else
                        {
                            parent->_right = cur->_right;
                        }
                        delete cur;
                    }
                    // 右为空
                    else if (cur->_right == nullptr)
                    {
                        if (parent->_left == cur)
                        {
                            parent->_left = cur->_left;
                        }
                        else
                        {
                            parent->_right = cur->_left;
                        }
                        delete cur;
                    }
                    // 左右都不为空
                    else if (cur->_left != nullptr && cur->_right != nullptr)
                    {
                        Node *rightMinP = cur;
                        Node *rightMin = cur->_right; // 看右子树的话，就一直想左走到头

                        while (rightMin->_left)
                        {
                            rightMinP = rightMin;
                            rightMin = rightMin->_left;
                        }

                        // 先把值赋给被删节点也就是cur，注意这里没有用交换，因为交换会破坏树的指针结构
                        cur->_key = rightMin->_key;

                        // 如果右子树的最左节点还有右子树的话，就把其右子树给到父节点
                        if (rightMinP->_left == rightMin)
                        {
                            rightMinP->_left = rightMin->_right;
                        }
                        else
                        {
                            rightMinP->_right = rightMin->_right;
                        }
                        delete rightMin;
                    }

                    return true;
                }
            }
            return false;
        }

        void Inorder()
        {
            _inorder(_root);
        }

        void Destroy(Node* root)
        {
            if (root == nullptr)
            {
                return;
            }

            Destroy(root->_left);
            Destroy(root->_right);

            delete root;
        }

        ~BSTree()
        {
            Destroy(_root);
            _root = nullptr;
        }

    private:
        Node *_root;
    };
}
