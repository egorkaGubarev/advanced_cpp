#include <thread>
#include "utils.h"

int main()
{
    std::cout << "Print '/exit' to close the program" << '\n';
    define_strings();
    std::thread sender(send_message, port1);
    std::thread receiver(receive_message, port2);
    sender.join();
    receiver.join();
    return 0;
}
