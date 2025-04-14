#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <memory>
#include <boost/asio.hpp>
#include "protocol.h"
#include "../Domain/TradingSystem.h"


class tcp_connection : public std::enable_shared_from_this<tcp_connection> {

public:

    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context, TradingSystem* tradingSystem)
    {
        return pointer(new tcp_connection(io_context, tradingSystem));
    }

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    //start async session
    void start();

private:

    tcp_connection(boost::asio::io_context& io_context, TradingSystem* trading_system) :
        socket_(io_context), tradingSystem_(trading_system)
    {}

    //initiate async read for an order request
    void do_read();

    void do_write(const OrderResponse& response);

    void process_message(const boost::system::error_code&, std::size_t message_size);

    void handle_write(const boost::system::error_code& err, std::size_t bytes_transferred);

    boost::asio::ip::tcp::socket socket_;
    TradingSystem* tradingSystem_;
    enum { max_length = 1024};
    char data_[max_length]; // buffer

};

#endif //TCP_CONNECTION_H
