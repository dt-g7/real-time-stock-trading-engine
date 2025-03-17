cmake_minimum_required(VERSION 3.31)
project(stocks)

set(CMAKE_CXX_STANDARD 20)

add_executable(stocks src/main.cpp
        src/Orderbook.cpp
)
