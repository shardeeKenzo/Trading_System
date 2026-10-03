cmake_minimum_required(VERSION 3.30)
project(BasicOrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_LINKER_TYPE LLD)

# Configure Boost preferences
set(Boost_USE_STATIC_LIBS ON)  # link boost statically
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.83 REQUIRED COMPONENTS system)

# Create your executable target (adjust the source file as needed)
add_executable(BasicOrderBook main.cpp
        Domain/Order.cpp
        Domain/Order.h
        Domain/Trade.cpp
        Domain/Trade.h
        Domain/OrderBook.cpp
        Domain/OrderBook.h
        Domain/MatchingEngine.cpp
        Domain/MatchingEngine.h
        Domain/TradingSystem.cpp
        Domain/TradingSystem.h
        Server/tcp_server.cpp
        Server/tcp_server.h
        Server/tcp_connection.cpp
        Server/tcp_connection.h
        Server/protocol.h)

target_compile_options(BasicOrderBook PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
# Link the Boost libraries (e.g., Boost.System)
target_link_libraries(BasicOrderBook PRIVATE Boost::system)
