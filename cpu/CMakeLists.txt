set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CPU_SRC CpuModule_Disassembler.c)

add_library(CPU_LIB ${CPU_SRC})
