#include <iostream>
#include <algorithm>
#include <initializer_list>

using std::cout;
using std::endl;

namespace my_list
{
    template <typename T>
    struct list_node
    {
        T _val;
        list_node<T>* _next;
        list_node<T>* _prev;

        list_node(T val = T()) // 一鱼两吃
        {
            _val = val;
            _next = nullptr;
            _prev = nullptr;
        }
    };

    ////////////////////////////////////////////////////迭代器封装//////////////////////////////////////////////////////
    template <typename T>
    struct list_iterator
    {
        typedef list_node<T> Node;
        Node* _node;


        list_iterator(Node* node)
        {
            _node = node;
        }

        T& operator*()
        {
            return _node->_val;
        }

        T* operator->()
        {
            return &(_node->_val);
        }

        list_iterator<T>& operator++() // 前置++
        {
            _node = _node->_next;
            return *this;              // 如果是隐式类型转换得到是临时对象具有常性，不能给左值引用
        }

        list_iterator<T>& operator--() // 前置--
        {
            _node = _node->_prev;
            return *this;          
        }

        list_iterator<T> operator++(int) // 后置++
        {
            Node* tmp = _node;
            _node = _node->_next;
            return list_iterator<T>(tmp);
        }

        bool operator!=(const list_iterator<T>& iterator) const
        {
            return _node != iterator._node;
        }

        bool operator==(const list_iterator<T>& iterator) const
        {
            return _node == iterator._node;
        }
    };

    template <typename T>
    struct list_const_iterator 
    {
        typedef list_node<T> Node;
        Node* _node;

        list_const_iterator(Node* node)
        {
            _node = node;
        }

        const T& operator*() const
        {
            return _node->_val;
        }

        const T* operator->() const
        {
            return &(_node->_val);
        }

        list_const_iterator<T>& operator++() // 前置++
        {
            _node = _node->_next;
            return *this;               // 如果是隐式类型转换得到是临时对象具有常性，不能给左值引用
        }

        list_const_iterator<T>& operator--() // 前置--
        {
            _node = _node->_prev;
            return *this;        
        }

        list_const_iterator<T> operator++(int) // 后置++
        {
            Node* tmp = _node;
            _node = _node->_next;
            return list_const_iterator<T>(tmp);
        }

        bool operator!=(const list_const_iterator<T>& iterator) const
        {
            return _node != iterator._node;
        }

        bool operator==(const list_const_iterator<T>& iterator) const
        {
            return _node == iterator._node;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <class T>
    class list
    {
        typedef list_node<T> Node;
        typedef list_iterator<T> iterator;
        typedef list_const_iterator<T> const_iterator;
    public:
        list()
        {
            empty_init();
        }

        void empty_init()
        {
            _head = new Node();
            _head->_next = _head;
            _head->_prev = _head;
        }

        list(const list<T>& other)
        {
            empty_init();

            for(const auto& e : other)
            {
                push_back(e);
            }
        }

        list(std::initializer_list<T> il)
        {
            empty_init();

            for(const auto& e : il)
            {
                push_back(e);
            }
        }

        void swap(list<T>& other)
        {
            std::swap(_head, other._head);
        }

        list<T>& operator=(list<T> other)
        {
            swap(other);
            return *this;
        }   

        list_iterator<T> begin()
        {
            return list_iterator<T>(_head->_next);
        }

        list_iterator<T> end()   // 注意是左闭右开区间，所以end()应该是哨兵位
        {
            return list_iterator<T>(_head);       // 显示类型转换更清晰
        }

        list_const_iterator<T> begin() const
        {
            return list_const_iterator<T>(_head->_next);
        }

        list_const_iterator<T> end() const
        {
            return list_const_iterator<T>(_head);
        }


        void push_back(const T& val)
        {
            insert(end(), val);
        }

        void push_front(const T& val)
        {
            insert(begin(), val);
        }

        void pop_back()
        {
            erase(--end());
        }

        void pop_front()
        {
            erase(begin());
        }

        void clear()
        {
            Node* cur = _head->_next;
            
            while (cur != _head)
            {
                Node* next = cur->_next;
                delete cur;
                cur = next;
            }

            _head->_next = _head;
            _head->_prev = _head;
        }

        iterator insert(iterator pos, const T& val) // 需要返回新插入位置的迭代器
        {
            Node* newnode = new Node(val);
            Node* cur = pos._node;
            Node* prev = cur->_prev;

            newnode->_next = cur;
            cur->_prev = newnode;
            prev->_next = newnode;
            newnode->_prev = prev;
            return iterator(newnode);
        }

        iterator erase(iterator pos)
        {
            Node* cur = pos._node;
            Node* next = cur->_next;
            Node* prev = cur->_prev;

            next->_prev = prev;
            prev->_next = next;
            delete cur;
            return iterator(next); // 返回被删的位置的下一个
        }

        size_t size() const
        {
            size_t count = 0;
            for (iterator it = begin(); it != end(); ++it)
            {
                count++;
            }
            return count;
        }

        bool empty() const
        {
            return _head == _head->_next;
        }

        ~list()
        {
            clear();
            delete _head;
        }
    private:
        Node* _head;
    };
}