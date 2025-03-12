#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <json/json.h>
#include <iostream>
#include <array>
#include <sstream>

using namespace boost::asio;
using namespace boost::asio::ip;

class UDPClient {
public:
    UDPClient(io_context& io, const std::string& addr, const std::string& port)
        : io_(io),
          socket_(io_, udp::endpoint(udp::v6(), 0)),
          resolver_(io_),
          remote_endpoint_(*resolver_.resolve(udp::v6(), addr, port).begin()) 
    {
        co_spawn(io_, sendText(), detached);
    }
<<<<<<< HEAD
    awaitable<void> sendtext()
    {
        std::string msg = "hello!!!";
        int len = co_await sk.async_send_to(buffer(msg), remote, use_awaitable);
        udp::endpoint sender;
        len = co_await sk.async_receive_from(buffer(buff), sender, use_awaitable);
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
=======
>>>>>>> 4816e37ff5c17b7779d9967fa9359bfa0853173a

private:
    awaitable<void> sendText() {
        try {
            std::string message = "hello!!!";
            co_await socket_.async_send_to(buffer(message), remote_endpoint_, use_awaitable);

            udp::endpoint sender;
            std::size_t len = co_await socket_.async_receive_from(buffer(buffer_), sender, use_awaitable);

            handleResponse(std::string(buffer_.data(), len));
        } 
        catch (const std::exception& e) {
            std::cerr << "Error in sendText: " << e.what() << std::endl;
        }
<<<<<<< HEAD
        std::cout << "done\n";
        while (true)
        {
            co_await receive_stuff();
=======

        while (true) {
            co_await receiveStuff();
        }
    }

    awaitable<void> receiveStuff() {
        try {
            udp::endpoint sender;
            std::size_t len = co_await socket_.async_receive_from(buffer(buffer_), sender, use_awaitable);
            std::string response(buffer_.data(), len);
            std::cout << "Received: " << response << std::endl;
        } 
        catch (const std::exception& e) {
            std::cerr << "Error in receiveStuff: " << e.what() << std::endl;
        }
    }

    void handleResponse(const std::string& response) {
        try {
            Json::Value jsonResponse;
            std::istringstream ss(response);
            ss >> jsonResponse;

            if (jsonResponse.isArray()) {
                for (const auto& entry : jsonResponse) {
                    if (!entry.isMember("ip") || !entry.isMember("port")) continue;

                    udp::socket tempSocket(io_, udp::endpoint(udp::v6(), 0));
                    udp::resolver tempResolver(io_);
                    udp::endpoint remote = *tempResolver.resolve(udp::v6(), entry["ip"].asString(), entry["port"].asString()).begin();

                    std::string message = "hello mfs!";
                    tempSocket.send_to(buffer(message), remote);
                }
            }
            std::cout << "Processing done.\n";
        } 
        catch (const std::exception& e) {
            std::cerr << "Error in handleResponse: " << e.what() << std::endl;
>>>>>>> 4816e37ff5c17b7779d9967fa9359bfa0853173a
        }
    }

private:
    io_context& io_;
    udp::socket socket_;
    udp::resolver resolver_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> buffer_;
};

int main() {
    try {
        io_context io;
        UDPClient client(io, "2409:40f4:2007:21cd:a019:f33b:815d:2454", "3000");
        io.run();
    } 
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
