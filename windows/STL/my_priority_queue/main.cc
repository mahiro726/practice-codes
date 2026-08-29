#include "priority_queue.hpp"
using namespace lcc;

template <class T>
class Less // 仿函数也就是函数对象
{
public:
    bool operator()(const T &x, const T &y)
    {
        return x < y;
    }
};

struct Date
{
    int _year;
    int _month;
    int _day;
};

struct GreaterDatePtr
{
    bool operator()(const Date *a, const Date *b) const
    {
        if (a->_year != b->_year)
        {
            return a->_year > b->_year;
        }
        if (a->_month != b->_month)
        {
            return a->_month > b->_month;
        }
        return a->_day > b->_day;
    }
};

std::ostream& operator<<(std::ostream& out, Date* date)
{
    out << date->_year << "-" << date->_month << "-" << date->_day;
    return out;
}

int main()
{
    priority_queue<Date *, std::vector<Date *>, GreaterDatePtr> pq;

    Date *d1 = new Date{2026, 8, 1};
    Date *d2 = new Date{2025, 12, 30};
    Date *d3 = new Date{2026, 8, 29};
    pq.push(d1);
    pq.push(d2);
    pq.push(d3);

    while (!pq.empty())
    {
        Date *top = pq.top();
        std::cout << top << std::endl;
        pq.pop();
        delete top; // 记得释放
    }
    return 0;
}