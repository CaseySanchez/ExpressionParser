cmake_minimum_required(VERSION 3.1)

project(WavefunctionExample)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WavefunctionExample main.cpp)

set_property(TARGET WavefunctionExample PROPERTY CXX_STANDARD 17)

target_link_libraries(WavefunctionExample ExpressionParser)
