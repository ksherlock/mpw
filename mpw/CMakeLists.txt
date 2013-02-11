set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -g")

add_definitions(-I ${CMAKE_SOURCE_DIR}/)

set(MPW_SRC mpw.cpp)



add_library(MPW_LIB ${MPW_SRC})