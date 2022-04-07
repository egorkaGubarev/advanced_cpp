#include <thread>
#include "utils.h"

int main()
{
    std::cout << "Print '/exit' to close the program" << '\n';
    define_strings();
    std::thread receiver(receive_message, port1);
    std::thread sender(send_message, port2);
    receiver.join();
    sender.join();
    return 0;
}
