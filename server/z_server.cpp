#include "z_server.hpp"
#define PORT 3000
#define buff_limit 1024
#include <iostream>
#include<json/json.h>
#include<sstream>
z_server::z_server(io_context &io) : server_sock(io, udp::endpoint(udp::v6(), PORT))
{
    listen();
}
awaitable<void> z_server::listen()
{
    std::array<char, buff_limit> buff;
    udp::endpoint remote;
    while (true)
    {
        memset(buff.begin(),1,buff.size());
        server_sock.async_receive_from(buffer(buff), remote, [&](boost::system::error_code ec, size_t len)
                                   {
            if (ec) {
                std::cerr << "failed: " << ec.message() << std::endl;
                return;
            } 
            Json::Value req;
            std::istringstream( std::string(buff.begin(),len))>>req;
            if (req["type"].asString()=="getpeer")
            {
                handle_getpeer(req["body"]);  
                return; 
            }
            if(req["type"].asString()=="validator-signup"){
                handle_validator_signup(req["body"]);
                return;
            }
         });
    }
    
    
}

awaitable<void> z_server::handle_getpeer(Json::Value body){
    int n = body["n"].asInt();
    Json::Value retval;
    if (ip_table.size()<n)
    {
        retval["status"]="not ok";

        server_sock.async_send_to(buffer(retval.asString()))
    }
    
    
}