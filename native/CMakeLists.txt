set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(NATIVE_SRC native.cpp file.cpp ${CMAKE_SYSTEM_NAME}.cpp)

add_library(NATIVE_LIB ${NATIVE_SRC})
target_link_libraries(NATIVE_LIB MACOS_LIB)
