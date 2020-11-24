cmake_minimum_required(VERSION 3.1)

project(ExpressionParserTest)

add_executable(ExpressionParserTest main.cpp)

set_property(TARGET ExpressionParserTest PROPERTY CXX_STANDARD 17)

target_link_libraries(ExpressionParserTest ExpressionParser)