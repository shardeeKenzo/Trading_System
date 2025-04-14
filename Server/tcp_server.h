#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <boost/asio.hpp>

#include "tcp_connection.h"

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context , TradingSystem* tradingSystem) :
        io_context_ { io_context },
        acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 5000) },
        tradingSystem_ { tradingSystem }
    {
        start_accept();
    }

private:

    void start_accept();
    void handle_accept(const tcp_connection::pointer& new_connection, const boost::system::error_code& error);


    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    TradingSystem* tradingSystem_;

};

#endif //TCP_SERVER_H
