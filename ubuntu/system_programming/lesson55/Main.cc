#include "Logger.hpp"
#include "ThreadPool.hpp"

#include <memory>

using namespace NS_LOG_MODULE;
using namespace NS_THREAD_MODULE;

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();

    std::unique_ptr<ThreadPool<int>> tp = std::make_unique<ThreadPool<int>>();
    tp->Start();

    tp->Wait();

    tp->Stop();
}