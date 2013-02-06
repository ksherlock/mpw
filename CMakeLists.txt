set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall  -g")

project(mpw)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(bin)
