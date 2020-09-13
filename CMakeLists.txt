cmake_minimum_required(VERSION 3.0)

project(ExpressionParser)

add_executable(ExpressionParser main.cpp node.cpp operation.cpp function.cpp expression_parser.cpp expression_composer.cpp expression_simplifier.cpp utils.cpp)

set_property(TARGET ExpressionParser PROPERTY CXX_STANDARD 20)
