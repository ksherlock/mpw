set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

#FILE(GLOB CPU_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


add_library(CPU_LIB ${CPU_SRC})
