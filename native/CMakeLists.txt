set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-I ${CMAKE_SOURCE_DIR}/)
add_definitions(-I ${CMAKE_BINARY_DIR}/)

set(NATIVE_SRC native.cpp ${CMAKE_SYSTEM_NAME}.cpp)

add_library(NATIVE_LIB ${NATIVE_SRC})
