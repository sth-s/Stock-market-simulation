cmake_minimum_required(VERSION 3.16)
project(advprog2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(LIB_NAME helper)
add_library(${LIB_NAME} 
    price_generator.cpp 
    input_parser.cpp
    stock_market.cpp
)

target_include_directories(${LIB_NAME} 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})