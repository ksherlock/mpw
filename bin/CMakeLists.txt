

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-unused-variable")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(
	OUTPUT lexer.cpp
	COMMAND "${RAGEL}" -p -G2 -o lexer.cpp "${CMAKE_CURRENT_SOURCE_DIR}/lexer.rl"
	MAIN_DEPENDENCY lexer.rl 
)

find_program(LEMON NAMES lemon)

if (LEMON)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
		# COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/parser.lemon" "parser.lemon"
		COMMAND ${LEMON} parser.lemon
		COMMAND mv -f parser.c parser.cpp
		MAIN_DEPENDENCY parser.lemon
		DEPENDS debugger.h
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/template_parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/template_parser.h
		# COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/template_parser.lemon" "template_parser.lemon"
		COMMAND ${LEMON} template_parser.lemon
		COMMAND mv -f template_parser.c template_parser.cpp
		MAIN_DEPENDENCY template_parser.lemon
		DEPENDS debugger.h
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)
endif()


add_custom_command(
	OUTPUT loadtrap.cpp
	COMMAND "${RAGEL}" -p -G2 -o loadtrap.cpp "${CMAKE_CURRENT_SOURCE_DIR}/loadtrap.rl"
	MAIN_DEPENDENCY loadtrap.rl 
	DEPENDS debugger.h
)


add_custom_command(
	OUTPUT template_loader.cpp
	COMMAND "${RAGEL}" -p -G2 -o template_loader.cpp "${CMAKE_CURRENT_SOURCE_DIR}/template_loader.rl"
	MAIN_DEPENDENCY template_loader.rl 
	DEPENDS debugger.h template_parser.h
)


add_executable(mpw loader.cpp debugger.cpp debugger_internal.cpp 
	address_map.cpp lexer.cpp parser.cpp loadtrap.cpp 
	commands.cpp
	template_loader.cpp template_parser.cpp intern.cpp template.cpp)


target_link_libraries(mpw CPU_LIB)
target_link_libraries(mpw TOOLBOX_LIB)
target_link_libraries(mpw MPW_LIB)
target_link_libraries(mpw MPLITE_LIB)
target_link_libraries(mpw MACOS_LIB)

target_link_libraries(mpw -ledit)
target_link_libraries(mpw "-framework Carbon")

add_executable(disasm disasm.cpp)
target_link_libraries(disasm CPU_LIB)
target_link_libraries(disasm MACOS_LIB)
target_link_libraries(disasm "-framework Carbon")

install(
  PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/mpw
  DESTINATION bin
)
