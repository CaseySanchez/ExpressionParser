cmake_minimum_required(VERSION 3.1)

project(SimplifyExample)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SimplifyExample main.cpp)

set_property(TARGET SimplifyExample PROPERTY CXX_STANDARD 17)

target_link_libraries(SimplifyExample ExpressionParser)