#include "tcp_server.h"

void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = tcp_connection::create(io_context_, tradingSystem_);

    acceptor_.async_accept(new_connection->socket(), [this, new_connection] (const boost::system::error_code& err)
    {
      handle_accept(new_connection, err);
    });
}

void tcp_server::handle_accept(const tcp_connection::pointer& new_connection, const boost::system::error_code& error)
{
    if (!error)
    {
        new_connection->start();
    } else {
        std::cerr << "Accept error: " << error.message() << "\n";
    }
    start_accept();
}