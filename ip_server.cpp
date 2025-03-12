#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <json/json.h>
using namespace boost::asio;
using namespace boost::asio::ip;

class UDPserver
{
public:
    UDPserver(io_context &io, int port) : me(io, ip::udp::endpoint(ip::udp::v6(), port))
    {
        startListening();
    }
    void startListening()
    {
     me.async_receive_from(buffer(buff), remote, [this](boost::system::error_code ec, std::size_t bytes_received) {
        handle_client(ec, bytes_received);
    });
    }

private:
    void handle_client(boost::system::error_code ec, size_t bytes_received)
    {
        if (!ec && bytes_received > 0)
        {
            std::cout << std::string(buff.data(), bytes_received) << "\n";
            Json::Value x(Json::arrayValue);
            for (auto &ip : ips)
            {
                Json::Value tmp;
                tmp["ip"] = ip.first;
                tmp["port"] = ip.second;
                x.append(tmp);
            }
            std::pair<std::string, std::string> tmp = {remote.address().to_string(), std::to_string(remote.port())};
            ips.push_back(tmp);
            std::cout << tmp.first << ", " << tmp.second << "\n";
            me.async_send_to(buffer(x.toStyledString()), remote, [](boost::system::error_code ec, std::size_t /*bytes_sent*/)
                             {
                    if (ec) {
                        std::cerr << "Send failed: " << ec.message() << std::endl;
                    } else {
                        std::cout << "Message sent successfully!" << std::endl;
                    } });
        }
        startListening();
    }
    ip::udp::endpoint remote;
    ip::udp::socket me;
    std::vector<std::pair<std::string, std::string>> ips;
    // boost::asio::buffer<char>  buff;
    std::array<char, 1024> buff;
};

int main()
{
    try
    {
        io_context io;
        UDPserver myserver(io, 3000);
        io.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}