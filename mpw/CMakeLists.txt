
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(MPW_SRC mpw.cpp mpw_io.cpp mpw_close.cpp mpw_access.cpp mpw_ioctl.cpp mpw_errno.cpp
	environment.cpp)


add_custom_command(
	OUTPUT environment.cpp
	COMMAND ragel -p -G2 -o environment.cpp "${CMAKE_CURRENT_SOURCE_DIR}/environment.rl"
	MAIN_DEPENDENCY environment.rl 
)


set_source_files_properties(
	environment.cpp # environ.cpp ep.cpp epv.cpp
	PROPERTIES
	COMPILE_FLAGS 
	"${CMAKE_CXX_FLAGS} -Wno-unused-variable"
)

add_library(MPW_LIB ${MPW_SRC})