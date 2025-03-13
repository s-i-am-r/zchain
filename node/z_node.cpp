#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/experimental/channel.hpp>
#include <queue>
#include <iostream>
#include <memory>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <mutex>
#include <sys/un.h> // For unlink()
using namespace boost::asio;
using namespace boost::asio::ip;
class Logger
{
public:
    std::ofstream logFile;
    bool newline = true;
    std::mutex logMutex; // Mutex for thread safety

    Logger(const std::string &filename)
    {
        logFile.open(filename, std::ios::app); // Open in append mode
    }

    ~Logger()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    template <typename T>
    Logger &operator<<(const T &message)
    {
        std::lock_guard<std::mutex> lock(logMutex); // Lock the mutex to prevent race conditions

        if (newline)
        {
            newline = false;
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::tm local_time = *std::localtime(&now_time);
            std::cout << std::put_time(&local_time, "[%Y-%m-%d %H:%M:%S] ");
            if (logFile.is_open())
            {
                logFile << std::put_time(&local_time, "[%Y-%m-%d %H:%M:%S] ");
            }
        }

        std::cout << message;
        if (logFile.is_open())
        {
            logFile << message;
        }

        return *this;
    }

    // Handle std::endl separately to ensure proper formatting
    Logger &operator<<(std::ostream &(*func)(std::ostream &))
    {
        std::lock_guard<std::mutex> lock(logMutex); // Lock the mutex to prevent race conditions

        func(std::cout);
        newline = true;
        if (logFile.is_open())
        {
            func(logFile);
        }
        return *this;
    }
};
Logger log_ = Logger("log.txt");

template <typename T>
class AsyncQueue
{
private:
    std::queue<T> queue;
    std::mutex mtx;
    boost::asio::experimental::channel<void(boost::system::error_code, int)> event_channel;

public:
    explicit AsyncQueue(thread_pool &pool)
        : event_channel(pool.get_executor(), 10) {} // Channel size 10

    void push(T value)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(value);
        }
        event_channel.try_send(boost::system::error_code(), value); // Notify consumer
    }

    awaitable<T> pop()
    {
        boost::system::error_code ec;
        T value;
        co_await event_channel.async_receive(redirect_error(use_awaitable, ec));

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!queue.empty())
            {
                value = queue.front();
                queue.pop();
            }
        }
        co_return value;
    }
};

class peer : public std::enable_shared_from_this<peer>
{
private:
    std::array<char, 1024> buff;

public:
    std::shared_ptr<tcp::socket> sock;
    void print()
    {
        log_ << "ip: " << sock->remote_endpoint().address().to_string() << "\nport: " << sock->remote_endpoint().port() << "\n";
    }
    peer(std::shared_ptr<tcp::socket> soc, thread_pool &pool) : sock(soc)
    {
        co_spawn(pool.executor(), start(), detached);
        log_ << "peer connected!" << std::endl;
    }

    awaitable<void> start()
    {
        while (true)
        {
            int len = co_await sock->async_receive(buffer(buff), use_awaitable);
            log_ << std::string(buff.begin(),len) << std::endl;
            std::string_view tmp(buff.data(), len);
            boost::json::value z = boost::json::parse(tmp);

            if (z.at("type").as_string() == "txn")
            {
                log_ << "txn received!: " << z.at("data").as_string() << std::endl;
            }
        }
    }
};

class z_node
{
private:
    std::vector<std::shared_ptr<peer>> peers;
    thread_pool &pool_;
    tcp::acceptor acceptor_;

public:
    z_node(thread_pool &pool, int port) : pool_(pool), acceptor_(pool.executor(), tcp::endpoint(ip::tcp::v6(), port))
    {
        co_spawn(pool.executor(), start_listening(), detached);
        co_spawn(pool.executor(), cli(), detached);
    }

    awaitable<void> start_listening()
    {
        log_ << "listening... at port " + acceptor_.local_endpoint().port() << std::endl;
        while (true)
        {
            auto client = std::make_shared<tcp::socket>(pool_.executor());
            co_await acceptor_.async_accept(*client, use_awaitable);
            auto p = std::make_shared<peer>(client, pool_);
            peers.push_back(p);
        }
    }
    awaitable<void> cli()
    {
        std::string name = "/tmp/z_node";
        int n = static_cast<int>(acceptor_.local_endpoint().port());
        log_ << n << std::endl;
        name += std::to_string(n);
        log_ << "cli server started " + name << std::endl;
        unlink(name.c_str());
        local::stream_protocol::acceptor acceptor(pool_.get_executor(), local::stream_protocol::endpoint(name));
        std::array<char, 1024> data;
        while (true)
        {
            local::stream_protocol::socket s(pool_.get_executor());
            acceptor.accept(s);

            size_t length = co_await s.async_receive(buffer(data), use_awaitable);
            std::string_view tmp(data.begin(), length);
            boost::json::value z = boost::json::parse(tmp);

            log_ << "command received. " << z.at("cmd").as_string() << std::endl;
            if (z.at("cmd").as_string() == "list clients")
            {
                int i = 1;
                log_ << "\n";
                for (auto &x : peers)
                {
                    log_ << "peer " << i;
                    x->print();
                    i++;
                }
                log_ << std::endl;
                continue;
            }
            if (z.at("cmd").as_string() == "add peer")
            {
                co_spawn(pool_.get_executor(), ([&]() -> awaitable<void>
                                                {
                            auto client = std::make_shared<tcp::socket>(pool_.executor());
                             co_await client->async_connect(tcp::endpoint(boost::asio::ip::address::from_string(std::string(z.at("ip").as_string())), z.at("port").as_int64()), use_awaitable);
                             auto p = std::make_shared<peer>(client, pool_);
                             peers.push_back(p); })(),
                         detached);
                continue;
            }
            if (z.at("cmd").as_string() == "send txn")
            {
                log_ << "inside" << std::endl;
                boost::json::value neww = boost::json::object();
                neww.as_object()["type"] = "txn";
                neww.as_object()["data"] = z.at("txn");
                std::string serialized_data = boost::json::serialize(neww);
                co_spawn(pool_.get_executor(), ([&]() -> awaitable<void>
                                                {
                    for(auto& peer:peers){
                        log_ << serialized_data << std::endl;
                        co_await peer->sock->async_write_some(buffer(serialized_data),use_awaitable);
                } })(),
                         detached);
            }
        }
    }
};

int main(int argc, char **argv)
{
    int port = 8080;
    if (argc > 1)
    {
        port = std::stoi(argv[1]);
    }

    boost::asio::thread_pool pool(4);
    z_node z(pool, port);
    pool.join(); // Run the thread pool
}
