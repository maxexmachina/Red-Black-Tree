cmake_minimum_required(VERSION 3.12)
project(ObjectTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(ObjectTree main.cpp rbTree.h rbTree.cpp)