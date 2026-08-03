#include "Shm.hpp"

int main()
{
    // 生命周期的代码级管理
    Shm sharedmem;
    sharedmem.Create();
    sharedmem.Attach();

    sharedmem.Attr();

    char* shm_start = (char*)sharedmem.Addr();
    int size = sharedmem.Size();

    while(true)
    {
        // 本质就是读取共享内存！
        for (int i = 0; i < size; i++)
        {
            std::cout << shm_start[i] << ' ';
        }
        std::cout << std::endl;
        sleep(2);
    }

    sharedmem.Detach();
    sharedmem.Delete();
    
}
