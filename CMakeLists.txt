cmake_minimum_required(VERSION 3.0)

project(ExpressionParser)

add_executable(ExpressionParser main.cpp term.cpp operation.cpp function.cpp expression_parser.cpp)

set_property(TARGET ExpressionParser PROPERTY CXX_STANDARD 20)

