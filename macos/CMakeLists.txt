set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(MACOS_SRC traps.c sysequ.c errors.cpp)


add_library(MACOS_LIB ${MACOS_SRC})
