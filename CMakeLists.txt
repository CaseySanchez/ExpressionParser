cmake_minimum_required(VERSION 2.8)

project(EquationParser)

add_executable(EquationParser main.cpp term.cpp operation.cpp function.cpp expression_parser.cpp)

set_property(TARGET EquationParser PROPERTY CXX_STANDARD 20)

target_link_libraries(EquationParser pcrecpp)
