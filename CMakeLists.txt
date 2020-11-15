cmake_minimum_required(VERSION 3.1)

project(ExpressionParser)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ExpressionParser SHARED node.cpp operations.cpp functions.cpp matrix.cpp expression_parser.cpp expression_composer.cpp expression_simplifier.cpp utils.cpp)

set_property(TARGET ExpressionParser PROPERTY CXX_STANDARD 17)

link_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(Examples)