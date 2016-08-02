set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wno-unused-function -g")

set(CPU_SRC 
	CpuModule.c 
	CpuModule_Disassembler.c 
	CpuModule_EffectiveAddress.c 
	CpuModule_Exceptions.c 
	CpuModule_Flags.c
	CpuModule_Logging.c
	CpuModule_Instructions.c
	CpuModule_InternalState.c
	CpuModule_Interrupts.c
	CpuModule_StackFrameGen.c
	memory.c
)

add_library(CPU_LIB ${CPU_SRC})
