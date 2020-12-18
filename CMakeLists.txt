cmake_minimum_required(VERSION 3.15)

project(ExpressionParser)

option(BUILD_EXAMPLES "Build the provided examples")
option(BUILD_TESTS "Build the provided unit tests")
option(BUILD_AWS "Build the AWS Lambda project")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ExpressionParser SHARED node.cpp operations.cpp functions.cpp matrix.cpp expression_parser.cpp expression_composer.cpp expression_simplifier.cpp equation_parser.cpp complex_parser.cpp expression_visualizer.cpp calculus.cpp utils.cpp)

set_target_properties(ExpressionParser PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

link_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

if (${BUILD_EXAMPLES})
    add_subdirectory(Examples)
endif()

if (${BUILD_TESTS})
    add_subdirectory(Test)
endif()

if (${BUILD_AWS})
    add_subdirectory(AWS)
endif()