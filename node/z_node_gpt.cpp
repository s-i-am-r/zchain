#include <boost/asio.hpp>
#include <json/json.h>
#include <boost/bind/bind.hpp>
#include <queue>
#include <iostream>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;
#define PORT 6969

class peer : public std::enable_shared_from_this<peer>
{
private:
    std::shared_ptr<tcp::socket> sock;
    std::queue<Json::Value> actionqueue;
    steady_timer timer;

public:
    explicit peer(std::shared_ptr<tcp::socket> socket)
        : sock(std::move(socket)), timer(sock->get_executor(), chrono::seconds(5))
    {
        start();
    }

    void start()
    {
        std::cout << "Peer connected!" << std::endl;
        timer.async_wait([&](const boost::system::error_code &ec)
                         { on_timer(ec); });
    }

    void on_timer(const boost::system::error_code &ec)
    {
        if (!ec)
        {
            std::cout << "hi" << std::endl;
            timer.expires_after(chrono::seconds(1));
            timer.async_wait([&](const boost::system::error_code &ec)
                             { on_timer(ec); });
        }
    }
};

class z_node
{
private:
    std::vector<std::shared_ptr<peer>> peers;
    io_context &io_;
    tcp::acceptor acceptor_;

public:
    z_node(io_context &io) : io_(io), acceptor_(io_, tcp::endpoint(tcp::v6(), PORT))
    {
        co_spawn(io_, start_listening(), detached);
    }

    awaitable<void> start_listening()
    {
        std::cout << "Listening... at port " << acceptor_.local_endpoint().port() << std::endl;
        while (true)
        {
            auto client = std::make_shared<tcp::socket>(io_);
            co_await acceptor_.async_accept(*client, use_awaitable);
            auto p = std::make_shared<peer>(client);
            peers.push_back(p);
        }
    }
};

int main()
{
    io_context io;
    z_node z(io);

    // 🔹 Keep `io_context` running
    io.run();  // Ensures the event loop does not exit

    return 0;
}
