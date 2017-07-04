set(CMAKE_INCLUDE_CURRENT_DIR ON)


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


#
# -ledit includes history stuff.  gnu -lreadline does not.
#

IF (ENABLE_DEBUGGER)
	set(DEBUGGER_SRC
		debugger.cpp debugger.cpp debugger_internal.cpp 
		address_map.cpp lexer.cpp parser.cpp loadtrap.cpp 
		commands.cpp
		template_loader.cpp template_parser.cpp intern.cpp template.cpp
	)
ENDIF()

add_executable(mpw main.cpp loadtrap.cpp ${DEBUGGER_SRC})

IF (ENABLE_DEBUGGER)

	if(HAVE_LIBEDIT)
		target_link_libraries(mpw edit)
	elseif(HAVE_LIBREADLINE)
		target_link_libraries(mpw readline)
		if (HAVE_LIBHISTORY)
		target_link_libraries(mpw history)
		endif()
	endif()


ENDIF()

if (MSVC)
	target_link_libraries(mpw WINLIB_LIB)
endif()

target_link_libraries(mpw CPU_LIB)
target_link_libraries(mpw TOOLBOX_LIB)
target_link_libraries(mpw MPW_LIB)
target_link_libraries(mpw MPLITE_LIB)
target_link_libraries(mpw NATIVE_LIB)
target_link_libraries(mpw MACOS_LIB)
target_link_libraries(mpw CXX_LIB)



#add_executable(disasm disasm.cpp)
#target_link_libraries(disasm CPU_LIB)
#target_link_libraries(disasm MACOS_LIB)

install(
  PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/mpw
  DESTINATION bin
)
