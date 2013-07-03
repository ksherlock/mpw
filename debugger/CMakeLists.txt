set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(DEBUGGER_SRC lexer.cpp parser.c)

add_custom_command(
	OUTPUT lexer.cpp
	COMMAND re2c -b -i -o lexer.cpp "${CMAKE_CURRENT_SOURCE_DIR}/lexer.re.cpp"
	MAIN_DEPENDENCY lexer.re.cpp 
	DEPENDS commands.h parser.h
)

add_custom_command(
	OUTPUT parser.c parser.h
	COMMAND cp -f "${CMAKE_CURRENT_SOURCE_DIR}/parser.lemon" "parser.lemon"
	COMMAND lemon parser.lemon
	COMMAND cp -f parser.h "${CMAKE_CURRENT_SOURCE_DIR}/"
	COMMAND cp -f parser.out "${CMAKE_CURRENT_SOURCE_DIR}/"
	MAIN_DEPENDENCY parser.lemon
	DEPENDS commands.h
)

add_library(DEBUGGER_LIB ${DEBUGGER_SRC})