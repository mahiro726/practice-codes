#include "Pipe.hpp"

int main()
{
    Fifo pipefile;
    pipefile.Buid();
    pipefile.Open(ForRead);

    std::string msg;

    while(true)
    {
        int n = pipefile.Recv(&msg);
        if (n > 0)
        {
            std::cout << "Client Say# " << msg << std::endl;
        }
        else
        {
            break;
        }
    }

    sleep(1);
    pipefile.Delete();
}