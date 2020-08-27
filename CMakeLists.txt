cmake_minimum_required(VERSION 2.8)

project(ExpressionParser)

add_executable(ExpressionParser main.cpp term.cpp operation.cpp function.cpp expression_parser.cpp)

set_property(TARGET ExpressionParser PROPERTY CXX_STANDARD 20)

target_link_libraries(ExpressionParser pcrecpp)
