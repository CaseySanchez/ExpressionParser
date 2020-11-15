cmake_minimum_required(VERSION 3.1)

project(MatrixExample)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MatrixExample main.cpp)

set_property(TARGET MatrixExample PROPERTY CXX_STANDARD 17)

target_link_libraries(MatrixExample ExpressionParser)