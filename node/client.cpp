#include <boost/asio.hpp>
#include <iostream>
#define PORT 8080
using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Connect to the server on localhost:12345
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("::"), PORT));

        // Send a message
        std::string message = "{\"type\":\"txn\", \
                            \"data\" : \"dummy txn \" \
                    }";
        boost::asio::write(socket, boost::asio::buffer(message));

        std::cout << "Message sent: " << message << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
