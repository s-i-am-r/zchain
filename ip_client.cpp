#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <iostream>
#include <array>
#include <json/json.h>
#include <sstream>
using namespace boost::asio;
using namespace boost::asio::ip;

class UDPclient
{
public:
    UDPclient(io_context &io, std::string addr, std::string port) : io_(io), sk(io_, ip::udp::endpoint(udp::v6(), 0)), resolver(io), remote(*resolver.resolve(udp::v6(), addr, port).begin())
    {
        co_spawn(io_, sendtext(), detached);
    }
    awaitable<void> sendtext()
    {
        std::string msg = "hello!!!";
        int len = co_await sk.async_send_to(buffer(msg), remote,use_awaitable);
        udp::endpoint sender;
        len = co_await sk.async_receive_from(buffer(buff), sender,use_awaitable);
        Json::Value xx;
        std::istringstream ss(std::string(buff.data(), len));
        ss >> xx;
        if (xx.isArray())
        {
            for (auto &x : xx)
            {
                udp::socket sk_(io_, udp::endpoint(udp::v6(), 0)); // Create UDP socket
                udp::resolver resolver_(io_);
                udp::endpoint remote_ = *resolver_.resolve(udp::v6(), x["ip"].asString(), x["port"].asString()).begin();

                // Send message
                std::string message = "hello mfs!";
                sk_.send_to(boost::asio::buffer(message), remote_);
            }
        }
        std::cout<<"done\n";
        while (true)
        {
            co_await receive_stuff();
        }
    }

private:
    awaitable<void> receive_stuff()
    {
        udp::endpoint sender;
        int len = co_await sk.async_receive_from(buffer(buff), sender);
        std::string ips(buff.data(), len);
        std::cout << ips << std::endl;
    }

    boost::asio::io_context &io_;
    ip::udp::socket sk;
    udp::resolver resolver;
    udp::endpoint remote;
    std::array<char, 1024> buff;
};

int main()
{
    try
    {
        io_context io;
        auto cli = UDPclient(io, "2409:40f4:200c:48ba:59ce:91db:636:b33", "3000");
        io.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}