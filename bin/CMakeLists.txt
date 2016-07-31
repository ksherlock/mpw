#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -Wno-deprecated-declarations -g")
SET(CMAKE_EXE_LINKER_FLAGS "-framework Carbon")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-I ${CMAKE_SOURCE_DIR}/)


# add_custom_command(
# 	OUTPUT lexer.cpp
# 	COMMAND re2c -b -i -o lexer.cpp "${CMAKE_CURRENT_SOURCE_DIR}/lexer.re.cpp"
# 	MAIN_DEPENDENCY lexer.re.cpp 
# 	DEPENDS debugger.h parser.h
# )

add_custom_command(
	OUTPUT lexer.cpp
	COMMAND ragel -p -G2 -o lexer.cpp "${CMAKE_CURRENT_SOURCE_DIR}/lexer.rl"
	MAIN_DEPENDENCY lexer.rl 
)

add_custom_command(
	OUTPUT parser.cpp parser.h
	COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/parser.lemon" "parser.lemon"
	COMMAND lemon parser.lemon
	COMMAND cp -f parser.h "${CMAKE_CURRENT_SOURCE_DIR}/"
	COMMAND cp -f parser.out "${CMAKE_CURRENT_SOURCE_DIR}/"
	COMMAND mv -f parser.c parser.cpp
	MAIN_DEPENDENCY parser.lemon
	DEPENDS debugger.h
)


add_custom_command(
	OUTPUT template_parser.cpp template_parser.h
	COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/template_parser.lemon" "template_parser.lemon"
	COMMAND lemon template_parser.lemon
	COMMAND cp -f template_parser.h "${CMAKE_CURRENT_SOURCE_DIR}/"
	COMMAND cp -f template_parser.out "${CMAKE_CURRENT_SOURCE_DIR}/"
	COMMAND mv -f template_parser.c template_parser.cpp
	MAIN_DEPENDENCY template_parser.lemon
	DEPENDS debugger.h
)



add_custom_command(
	OUTPUT loadtrap.cpp
	COMMAND ragel -p -G2 -o loadtrap.cpp "${CMAKE_CURRENT_SOURCE_DIR}/loadtrap.rl"
	MAIN_DEPENDENCY loadtrap.rl 
	DEPENDS debugger.h
)


add_custom_command(
	OUTPUT template_loader.cpp
	COMMAND ragel -p -G2 -o template_loader.cpp "${CMAKE_CURRENT_SOURCE_DIR}/template_loader.rl"
	MAIN_DEPENDENCY template_loader.rl 
	DEPENDS debugger.h template_parser.h
)



set_source_files_properties(
	loadtrap.cpp lexer.cpp template_loader.cpp
	PROPERTIES
	COMPILE_FLAGS
	"${CMAKE_CXX_FLAGS} -Wno-unused-variable"
)

add_executable(mpw loader.cpp debugger.cpp debugger_internal.cpp 
	address_map.cpp lexer.cpp parser.cpp loadtrap.cpp 
	commands.cpp
	template_loader.cpp template_parser.cpp intern.cpp template.cpp)


target_link_libraries(mpw CPU_LIB)
target_link_libraries(mpw TOOLBOX_LIB)
target_link_libraries(mpw MPW_LIB)
target_link_libraries(mpw MPLITE_LIB)
target_link_libraries(mpw NATIVE_LIB)
target_link_libraries(mpw MACOS_LIB)
set_target_properties(mpw PROPERTIES LINK_FLAGS "-ledit")


add_executable(disasm disasm.cpp)
target_link_libraries(disasm CPU_LIB)
target_link_libraries(disasm MACOS_LIB)

install(
  PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/mpw
  DESTINATION bin
)
