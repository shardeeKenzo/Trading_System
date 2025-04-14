cmake_minimum_required(VERSION 3.30)
project(Trading_System)

set(CMAKE_CXX_STANDARD 26)

# Create your executable target (adjust the source file as needed)
add_executable(Trading_System main.cpp
        Domain/Order.cpp
        Domain/Order.h
        Domain/Trade.cpp
        Domain/Trade.h
        Domain/OrderBook.cpp
        Domain/OrderBook.h
        Domain/MatchingEngine.cpp
        Domain/MatchingEngine.h
        Domain/TradingSystem.cpp
        Domain/TradingSystem.h)

