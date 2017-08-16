cmake_minimum_required(VERSION 3.7)
project(TestExpressionTemplate)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Expression.hpp)
add_executable(TestExpressionTemplate ${SOURCE_FILES})