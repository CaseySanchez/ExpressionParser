cmake_minimum_required(VERSION 3.0)

project(ExpressionParserTest)

add_executable(ExpressionParserTest test_expression_parser.cpp ../node.cpp ../operation.cpp ../function.cpp ../expression_parser.cpp ../expression_composer.cpp ../expression_simplifier.cpp ../utils.cpp)

set_property(TARGET ExpressionParserTest PROPERTY CXX_STANDARD 20)
