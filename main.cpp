#include <boost/asio.hpp>
#include "Server/tcp_server.h"
#include "Domain/TradingSystem.h"
#include "Domain/OrderBook.h"
#include "Domain/MatchingEngine.h"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        // Create core trading objects.
        OrderBook orderBook;
        MatchingEngine matchingEngine;
        TradingSystem tradingSystem(orderBook, matchingEngine);

        // Instantiate the TCP server with our trading system.
        tcp_server server(io_context, &tradingSystem);

        std::cout << "Server running on port 5000..." << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

