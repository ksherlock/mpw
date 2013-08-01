set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -g")

add_definitions(-I ${CMAKE_SOURCE_DIR}/)

set(MPW_SRC mpw.cpp mpw_io.cpp mpw_close.cpp mpw_access.cpp mpw_ioctl.cpp
	environ.cpp epv.cpp ep.cpp)

add_custom_command(
	OUTPUT environ.cpp
	COMMAND ragel -p -G2 -o environ.cpp "${CMAKE_CURRENT_SOURCE_DIR}/environ.rl"
	MAIN_DEPENDENCY environ.rl 
)


add_custom_command(
	OUTPUT epv.cpp
	COMMAND ragel -p -G2 -o epv.cpp "${CMAKE_CURRENT_SOURCE_DIR}/epv.rl"
	MAIN_DEPENDENCY epv.rl 
)


add_custom_command(
	OUTPUT ep.cpp
	COMMAND ragel -p -G2 -o ep.cpp "${CMAKE_CURRENT_SOURCE_DIR}/ep.rl"
	MAIN_DEPENDENCY ep.rl 
)




set_source_files_properties(
	environ.cpp ep.cpp epv.cpp
	PROPERTIES
	COMPILE_FLAGS 
	"${CMAKE_CXX_FLAGS} -Wno-unused-variable"
)

add_library(MPW_LIB ${MPW_SRC})