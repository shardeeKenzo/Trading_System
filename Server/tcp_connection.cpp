#include "tcp_connection.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

void tcp_connection::start() {
    do_read();
}

/*void tcp_connection::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(data_, sizeof(Header)),
        [this, self](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handle_read(ec, bytes_transferred);
        });
}*/

void tcp_connection::do_read() {
    auto self(shared_from_this());

    boost::asio::async_read(socket_,
        boost::asio::buffer(data_, sizeof(Header)),
        [this, self](const boost::system::error_code& ec, std::size_t) {
            if(!ec) {
                const Header* header = reinterpret_cast<Header*>(data_);
                std::size_t expected_size = 0;

                switch (header->command) {
                    case 1:
                        expected_size = sizeof(OrderAdd);
                        break;
                    case 2:
                        expected_size = sizeof(OrderModify);
                        break;
                    case 3:
                        expected_size = sizeof(OrderCancel);
                        break;
                    default:
                        std::cerr << "Unknown command: " << static_cast<int>(header->command) << "\n";
                        do_write({.accepted = 1, .status = 1, .order_id = 0});
                    return;
                }
                if (expected_size > sizeof(Header))
                {
                    boost::asio::async_read(socket_,
                        boost::asio::buffer(data_ + sizeof(Header), expected_size - sizeof(Header)),
                        [this, self, expected_size](const boost::system::error_code& ec2, std::size_t)
                        {
                           if (!ec2)
                           {
                               process_message(ec2, expected_size);
                           } else
                           {
                               std::cerr << "Payload read error: " << ec2.message() << "\n";
                           }
                        });
                } else
                {
                    process_message(ec, expected_size);
                }
            } else
            {
                std::cerr << "Header read error: " << ec.message() << "\n";
            }
    });
}

void tcp_connection::process_message(const boost::system::error_code& ec, std::size_t message_size)
{
    if (!ec)
    {
        const Header* msg = reinterpret_cast<Header*>(data_);
        OrderResponse resp;

        switch (msg->command)
        {
        case 1:
            {
                OrderAdd* order_add = reinterpret_cast<OrderAdd*>(data_);
                ProcessResult p_result = tradingSystem_->process(*order_add);

                resp.accepted = p_result.accepted_;
                resp.status = 0;
                resp.order_id = p_result.order_id;
                break;
            }
        case 2:
            {
                OrderModify* order_modify = reinterpret_cast<OrderModify*>(data_);
                ProcessResult p_result {};
                try
                {
                    p_result = tradingSystem_->process(*order_modify);
                } catch (const std::invalid_argument& e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    resp.accepted = 1;
                    resp.status = 1;
                    resp.order_id = 0;
                    break;
                }

                resp.accepted = p_result.accepted_;
                resp.status = 0;
                resp.order_id = p_result.order_id;
                break;
            }
        case 3:
            {
                OrderCancel* order_cancel = reinterpret_cast<OrderCancel*>(data_);
                ProcessResult p_result {};

                try
                {
                   p_result = tradingSystem_->process(*order_cancel);
                } catch (const std::invalid_argument& e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    resp.accepted = 1;
                    resp.status = 1;
                    resp.order_id = 0;
                    break;
                }

                resp.accepted = p_result.accepted_;
                resp.status = 0;
                resp.order_id = p_result.order_id;
                break;
            }
        }

        do_write(resp);
    } else
    {
        std::cerr << "Error: " << ec.message() << "\n";
    }
}

void tcp_connection::do_write(const OrderResponse& response) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(&response, sizeof(OrderResponse)),
        [this, self](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handle_write(ec, bytes_transferred);
        });
}

void tcp_connection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        do_read();
    } else {
        std::cerr << "Write error: " << error.message() << "\n";
    }
}




