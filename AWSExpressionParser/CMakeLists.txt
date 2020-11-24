cmake_minimum_required(VERSION 3.1)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

project(AWSExpressionParser)

find_package(AWSSDK REQUIRED COMPONENTS lambda)
find_package(aws-lambda-runtime REQUIRED)

add_executable(AWSExpressionParser main.cpp)

set_property(TARGET AWSExpressionParser PROPERTY CXX_STANDARD 17)

target_link_libraries(AWSExpressionParser PUBLIC ExpressionParser ${AWSSDK_LINK_LIBRARIES} AWS::aws-lambda-runtime)

aws_lambda_package_target(${PROJECT_NAME})