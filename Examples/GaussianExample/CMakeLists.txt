cmake_minimum_required(VERSION 3.1)

project(GaussianExample)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GaussianExample main.cpp)

set_property(TARGET GaussianExample PROPERTY CXX_STANDARD 17)

target_link_libraries(GaussianExample ExpressionParser)
