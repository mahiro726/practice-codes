#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

//用户指明的
#define PATHNAME "/tmp"
#define PROJ_ID 0x66
const int gsize = 128;

class Shm
{
public:
    Shm()
    :_shmid(-1),
    _size(gsize),
    _start_addr(nullptr)
    {}


    // 1. 创建共享内存
    void Create()
    {
        GetHelper(IPC_CREAT | IPC_EXCL | 0666); // 复用GetHelper，仅传flag，用以区分
    }

    // 1. 得到Server创建好的共享内存
    void Get()
    {
        GetHelper(IPC_CREAT);
    }

    // 2. 删除共享内存
    void Delete()
    {
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        (void)n;
    }

    // 3. 关联共享内存
    void Attach()
    {
        _start_addr = shmat(_shmid, nullptr, 0);
        if ((long long int)_start_addr == -1)
        {
            perror("shmat error: ");
            exit(3);
        }
    }

    // 4. 去关联共享内存
    void Detach()
    {
        int n = shmdt(_start_addr);
        (void)n;
    }

    void Attr()
    {
        struct shmid_ds ds;
        int n = shmctl(_shmid, IPC_STAT, &ds);
        if (n < 0)
        {
            perror("shmctl error: ");
            exit(4);
        }
        printf("key: 0x%x\n", ds.shm_perm.__key);
        printf("shm_nttach: %ld\n", ds.shm_nattch);
        printf("shm_segsz: 0x%lx\n", ds.shm_segsz);
    }

    void* Addr()
    {
        return _start_addr;
    }
    
    int Size()
    {
        return _size;
    }
    
    ~Shm()
    {}
private:
    key_t GetKey()
    {
        return ftok(PATHNAME, PROJ_ID);
    }

    void GetHelper(int shmflg)
    {
        // 1. 构建减值
        key_t k = GetKey();
        if(k < 0)
        {
            std::cerr << "GetKey error";
            exit(1);
        }
        printf("GetKey() success\n");

        // 2. 创建新的共享内存
        _shmid = shmget(k, _size, shmflg);
        if (_shmid < 0)
        {
            perror("shmget error");
            exit(2);
        }
        printf("key=0x%x, _shmid = %d\n", k, _shmid);
    }


private:
    int _shmid;
    int _size;
    void* _start_addr;
};