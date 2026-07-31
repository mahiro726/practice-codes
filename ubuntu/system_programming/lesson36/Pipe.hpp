#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ForRead 1
#define ForWrite 2
const std::string gcommfile = "./fifo";

class Fifo
{
public:
    Fifo(const std::string &commfile = gcommfile)
        : _commfile(commfile),
          _mode(0666)
    {
    }
    // 1. 创建管道
    void Buid()
    {
        if (IsExists())
            return;

        umask(0);

        int n = mkfifo(_commfile.c_str(), _mode);
        if (n < 0)
        {
            std::cerr << "mkfifo error: " << strerror(errno) << " errno: " << errno << std::endl;
            exit(1);
        }
        std::cerr << "mkfifo success: " << strerror(errno) << " errno: " << errno << std::endl;
    }
    // 2. 打开管道
    void Open(int mode)
    {
        // 在通信没有开始之前，如果读端打开，写端如果没有打开，读端open就会阻塞，直到write打开
        // 为什么？区分对端写关闭的情况或者读关闭
        if (mode == ForRead)
        {
            _fd = open(_commfile.c_str(), O_RDONLY);
        }
        else if(mode == ForWrite)
        {
            _fd = open(_commfile.c_str(), O_WRONLY);
        }

        if (_fd < 0)
        {
            std::cerr << "open error: " << strerror(errno) << " errno: " << errno << std::endl;
            exit(2);
        }
        else
        {
            std::cout << "open file success" << std::endl;
        }
    }

    // 3.发送信息以及接收信息
    void Send(const std::string &msgin)
    {
        ssize_t n = write(_fd, msgin.c_str(), msgin.size());
        (void)n;
    }

    int Recv(std::string *msgout)
    {
        char buffer[128];
        ssize_t n = read(_fd, buffer, sizeof(buffer)-1); // 为什么-1？因为read不关心是不是字符串，需要人为添加'\n'，-1是为了留空间
        if (n > 0)
        {
            buffer[n] = 0;
            *msgout = buffer;
            return n;
        }
        else if(n == 0)
        {
            return 0;
        }
        else 
        {
            return -1; 
        }
    }

    // 4. 删除管道
    void Delete()
    {
        if (IsExists())
        {
            int n = unlink(_commfile.c_str());
            (void)n;
            std::cout << "unlink " << _commfile << std::endl;
        }
    }

    ~Fifo()
    {
    }

private:
    bool IsExists()
    {
        // 因为管道特点，不能使用open（会阻塞）
        struct stat st;
        int n = stat(_commfile.c_str(), &st);
        if (n == 0)
        {
            return true;
        }
        else
        {
            errno = 0;
            return false;
        }
    }

private:
    std::string _commfile;
    mode_t _mode;
    int _fd;
};