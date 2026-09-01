#include "RBTree.hpp"
#include <vector>

using namespace lcc;

int main()
{
    std::vector<std::vector<int>> cases = 
    {
        {5,3,2}, {1,3,5}, {5,2,3}, {2,5,3}, {10,5,15,3}
    };


    for (auto& a : cases)
    {
        RBTree<int,int> t;
        for (int x : a) t.Insert(std::make_pair(x,x));
        t.Inorder();          // 中序应有序
        std::cout << t.IsValidRBTree() << std::endl;   // 应全输出 1
    }
}