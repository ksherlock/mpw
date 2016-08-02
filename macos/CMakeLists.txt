set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-I ${CMAKE_SOURCE_DIR}/)

set(MACOS_SRC traps.c sysequ.c errors.cpp)


add_library(MACOS_LIB ${MACOS_SRC})
