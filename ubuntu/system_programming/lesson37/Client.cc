#include "Shm.hpp"

int main()
{
    // 不需要创建内核级共享内存
    Shm sharedmem;
    sharedmem.Get();
    sharedmem.Attach();

    char* shm_start = (char*)sharedmem.Addr();
    int size = sharedmem.Size();
    int index = 0;
    while(true)
    {
        std::cout << "Please Enter@ ";
        std::cin >> *shm_start;
        // char ch;
        // std::cin >> ch;
        // shm_start[index++] = ch;
        shm_start++;

        index %= size;
    }

    sharedmem.Detach();
}
