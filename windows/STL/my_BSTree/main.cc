#include "BSTree.hpp"

using namespace lcc;

int main()
{
    BSTree<int> t;
    int a[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for (int x : a)
        t.Insert(x);
    t.Inorder(); // 1 3 4 6 7 8 10 13 14

    std::cout << std::endl;

    t.Erase(8);  // 删根（双孩子）
    t.Inorder(); // 1 3 4 6 7 10 13 14
    std::cout << std::endl;

    t.Erase(14); // 删叶子
    t.Inorder(); // 1 3 4 6 7 10 13
    std::cout << std::endl;

    t.Erase(3);  // 删双孩子
    t.Inorder(); // 1 4 6 7 10 13
    std::cout << std::endl;

    t.Erase(1);  // 删叶子（根的直接左孩子）
    t.Inorder(); // 4 6 7 10 13

    return 0;
}