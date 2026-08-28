#include "List.hpp"

using namespace my_list;

template <class T>
void print(const list<T>& lt)
{
    typename list_iterator it = lt.begin();
    while(it != end())
    {
        cout << *it << " ";
        ++it;
    } 
}

int main()
{
    list<int> lt;
    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    for (auto& e : lt)
    {
        std::cout << e << " ";
    }

    return 0;
}