#ifndef EIGHTH_SEM_UTILS_H
#define EIGHTH_SEM_UTILS_H

#include <boost/asio.hpp>
#include <iostream>
#include <mutex>

typedef unsigned short int ushort;

const ushort port1 = 10'000;
const ushort port2 = 10'001;

const char delimiter = '\n';

std::string connection_message, disconnect_signal, ip_str, receiver_finish_message, receiver_start_message,
receiver_wait_message, sender_finish_message, sender_start_message, sender_wait_message, switch_signal;

std::mutex mutex;

void define_strings()
{
    connection_message = "/connect";
    disconnect_signal = "/exit";
    ip_str = "127.0.0.1";
    receiver_finish_message = "Stop receiving";
    receiver_start_message = "Ready to receive";
    receiver_wait_message = "Waiting for sender";
    sender_finish_message = "Stop sending";
    sender_start_message = "Ready to send";
    sender_wait_message = "Print '/connect' when receiver is ready";
    switch_signal = "/switch";
}

bool read(boost::asio::ip::tcp::socket& socket, std::string& message)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, delimiter);
    std::istream stream(&buffer);
    std::getline(stream, message);
    return message != disconnect_signal && message != switch_signal;
}

void receive_message(const ushort port)
{
    const boost::asio::ip::address ip = boost::asio::ip::address::from_string(ip_str);
    boost::asio::ip::tcp::endpoint endpoint(ip, port);
    boost::asio::io_service io_service;
    const boost::asio::ip::tcp protocol = endpoint.protocol();
    boost::asio::ip::tcp::acceptor acceptor(io_service, protocol);
    acceptor.bind(endpoint);
    acceptor.listen();
    mutex.lock();
    std::cout << receiver_wait_message << '\n';
    mutex.unlock();
    boost::asio::ip::tcp::socket socket(io_service);
    acceptor.accept(socket);
    mutex.lock();
    std::cout << receiver_start_message << '\n';
    mutex.unlock();
    std::string message;
    while (read(socket, message)) {
        mutex.lock();
        std::cout << message << std::endl;
        mutex.unlock();
    }
    mutex.lock();
    std::cout << receiver_finish_message << '\n';
    mutex.unlock();
}

void send_message(const ushort port)
{
    try {
        const boost::asio::ip::address ip = boost::asio::ip::address::from_string(ip_str);
        boost::asio::ip::tcp::endpoint endpoint(ip, port);
        boost::asio::io_service io_service;
        const boost::asio::ip::tcp protocol = endpoint.protocol();
        boost::asio::ip::tcp::socket socket(io_service, protocol);
        mutex.lock();
        std::cout << sender_wait_message << '\n';
        mutex.unlock();
        std::string message;
        std::getline(std::cin, message);
        while(message != connection_message){
            std::getline(std::cin, message);
        }
        socket.connect(endpoint);
        mutex.lock();
        std::cout << sender_start_message << '\n';
        mutex.unlock();
        message = "";
        while(message != disconnect_signal && message != switch_signal){
            std::getline(std::cin, message);
            const std::string message_with_delimiter = message + delimiter;
            const boost::asio::const_buffers_1 buffer = boost::asio::buffer(message_with_delimiter);
            boost::asio::write(socket, buffer);
        }
        mutex.lock();
        std::cout << sender_finish_message << '\n';
        mutex.unlock();
    }
    catch (const std::system_error &err) {
        const char* const description = err.what();
        mutex.lock();
        std::cerr << description;
        mutex.unlock();
    }
}

#endif
