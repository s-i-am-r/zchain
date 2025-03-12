#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/experimental/channel.hpp>
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

using namespace boost::asio;
using namespace std;

class AsyncQueue {
private:
    std::queue<int> queue;
    std::mutex mtx;
    boost::asio::experimental::channel<void(boost::system::error_code, int)> event_channel;

public:
    explicit AsyncQueue(thread_pool &pool)
        : event_channel(pool.get_executor(), 10) {}  // Channel size 10

    void push(int value) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(value);
        }
        event_channel.try_send(boost::system::error_code(), value); // Notify consumer
    }

    awaitable<int> pop() {
        boost::system::error_code ec;
        int value;
        co_await event_channel.async_receive(redirect_error(use_awaitable, ec));
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!queue.empty()) {
                value = queue.front();
                queue.pop();
            }
        }
        co_return value;
    }
};

// Global objects
thread_pool pool(2);
AsyncQueue q(pool);

// Producer
awaitable<void> producer(int n) {
    auto exec = co_await this_coro::executor;
    steady_timer t(exec, boost::asio::chrono::seconds(1));
    co_await t.async_wait(use_awaitable);
    std::cout << "Pushed by thread: " << std::this_thread::get_id() << std::endl;
    q.push(n);
}

// Consumer
awaitable<void> consumer() {
    
    co_spawn(pool,[&]()->awaitable<void>{
        int value = co_await q.pop();
        std::cout << "Popped by thread: " << std::this_thread::get_id() << " -> " << value << std::endl;
        co_return;
    },detached);
    
    steady_timer t(pool,boost::asio::chrono::milliseconds(500));
    while (1)
    {
        std::cout<<"hi"<<std::endl;
        co_await t.async_wait(use_awaitable);
        t.expires_after(boost::asio::chrono::milliseconds(500));
    }
    
}

int main() {
    co_spawn(pool, producer(730), detached);
    co_spawn(pool, consumer(), detached);

    pool.join(); // Wait for all tasks to complete
}
