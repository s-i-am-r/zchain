#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class P
{
public:
    boost::asio::steady_timer t;
    int count;
    P(boost::asio::io_context&io):t(io, boost::asio::chrono::seconds(1)),count(0){
        t.async_wait([&](const boost::system::error_code & e){
                print(e);
            });
    }
    void print(const boost::system::error_code & /*e*/)
    {
        // if (count < 5000000)
        // {
            std::cout << count << std::endl;
            // ++(count);

            t.expires_at(t.expiry() + boost::asio::chrono::seconds(1));
            t.async_wait([&](const boost::system::error_code & e){
                print(e);
            });
        // }
    }
};

int main()
{
    boost::asio::io_context io;
    P p(io);
    

    io.run();
    std::cout << "Final count is " << p.count << std::endl;


    return 0;
}