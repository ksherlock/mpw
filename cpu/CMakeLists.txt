if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-function")
endif()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	set(CMAKE_C_FLAGS "-Wall -Wno-unused-function")
endif()

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
