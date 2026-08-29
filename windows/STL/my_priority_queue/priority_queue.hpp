#pragma once
#include <iostream>
#include <vector>

namespace lcc
{
    template <class T, class Container = std::vector<T>, class Compare = std::greater<T>>
    class priority_queue
    {
    public:
        priority_queue() = default;

        template <class Inputiterator>
        priority_queue(Inputiterator first, Inputiterator last)
        :_con(first, last)
        {
            for(int i = ((int)_con.size() - 2) / 2; i >= 0; i--) // 如果i为size_t，会有死循环的问题
            {
                adjust_down(i); // 向下调整建堆
            }
        }

        void adjust_up(size_t child)
        {
            while (child > 0)
            {
                size_t parent = (child-1) / 2;

                if (_cmp(_con[parent], _con[child]))
                {
                    std::swap(_con[parent], _con[child]);
                }
                else
                {
                    break;
                }
                child = parent;
            }
        }

        void adjust_down(size_t parent)
        {
            size_t child = parent*2 + 1; // 默认左孩子
            while (child < _con.size())  // 进循环的时候直接判断是否孩子存在
            {
                if (child+1 < _con.size() && _cmp(_con[child], _con[child+1]))
                {
                    child = child+1;
                }

                if (_cmp(_con[parent], _con[child]))
                {
                    std::swap(_con[child], _con[parent]);
                }
                else
                {
                    break;
                }

                parent = child;
                child = parent*2+1; // 默认左孩子
            }

        }

        void push(const T& val)
        {
            _con.push_back(val);
            
            adjust_up(_con.size()-1);
        }

        const T& top() const
        {
            return _con.front();
        }

        void pop()
        {
            std::swap(_con.front(), _con.back()); // 先首尾交换，将要pop出去的移到尾巴上
            _con.pop_back(); // 然后pop尾

            adjust_down(0);
        }

        void swap(Container& con, Compare& cmp)
        {
            std::swap(con, _con);
            std::swap(cmp, _cmp);
        }

        size_t size() const
        {
            return _con.size();
        }

        bool empty() const
        {
            return _con.empty();
        }

        ~priority_queue(){}
    private:
        Container _con;
        Compare _cmp;
    };
}