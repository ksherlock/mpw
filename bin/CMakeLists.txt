#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -Wno-deprecated-declarations -g")
SET(CMAKE_EXE_LINKER_FLAGS "-framework Carbon")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-I ${CMAKE_SOURCE_DIR}/)


add_custom_command(
	OUTPUT lexer.cpp
	COMMAND re2c -b -i -o lexer.cpp "${CMAKE_CURRENT_SOURCE_DIR}/lexer.re.cpp"
	MAIN_DEPENDENCY lexer.re.cpp 
	DEPENDS debugger.h parser.h
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


add_executable(mpw loader.cpp debugger.cpp address_map.cpp lexer.cpp parser.cpp)
target_link_libraries(mpw CPU_LIB)
target_link_libraries(mpw TOOLBOX_LIB)
target_link_libraries(mpw MPW_LIB)
target_link_libraries(mpw MPLITE_LIB)
target_link_libraries(mpw MACOS_LIB)
set_target_properties(mpw PROPERTIES LINK_FLAGS "-ledit")


add_executable(disasm disasm.cpp)
target_link_libraries(disasm CPU_LIB)
target_link_libraries(disasm MACOS_LIB)
