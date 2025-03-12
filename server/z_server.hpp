#if !defined(z_server)

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
class nodeinfo
{
public:
    std::string pk;
    std::string ip;
    std::string port;
};
class z_server
{
private:
    std::vector<nodeinfo> ip_table;
    udp::socket server_sock;

public:
    z_server(io_context &);
    awaitable<void> listen();
    awaitable<void> handle_getpeer(Json::Value);
    awaitable<void> handle_validator_signup(Json::Value);
};

#endif // z_server
