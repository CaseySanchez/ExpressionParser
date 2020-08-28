cmake_minimum_required(VERSION 3.0)

project(ExpressionParserTest)

add_executable(ExpressionParserTest test_expression_parser.cpp ../term.cpp ../operation.cpp ../function.cpp ../expression_parser.cpp)

set_property(TARGET ExpressionParserTest PROPERTY CXX_STANDARD 20)
