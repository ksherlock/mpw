set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -g")

add_definitions(-I ${CMAKE_SOURCE_DIR}/)

set(MACOS_SRC traps.c sysequ.c errors.cpp)


add_library(MACOS_LIB ${MACOS_SRC})