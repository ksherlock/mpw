set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(MPW_SRC mpw.cpp mpw_io.cpp mpw_close.cpp mpw_access.cpp mpw_ioctl.cpp mpw_errno.cpp
	environment.cpp)

# add_custom_command(
# 	OUTPUT environ.cpp
# 	COMMAND ragel -p -G2 -o environ.cpp "${CMAKE_CURRENT_SOURCE_DIR}/environ.rl"
# 	MAIN_DEPENDENCY environ.rl 
# )

add_custom_command(
	OUTPUT environment.cpp
	COMMAND ragel -p -G2 -o environment.cpp "${CMAKE_CURRENT_SOURCE_DIR}/environment.rl"
	MAIN_DEPENDENCY environment.rl 
)


# add_custom_command(
# 	OUTPUT epv.cpp
# 	COMMAND ragel -p -G2 -o epv.cpp "${CMAKE_CURRENT_SOURCE_DIR}/epv.rl"
# 	MAIN_DEPENDENCY epv.rl 
# )


# add_custom_command(
# 	OUTPUT ep.cpp
# 	COMMAND ragel -p -G2 -o ep.cpp "${CMAKE_CURRENT_SOURCE_DIR}/ep.rl"
# 	MAIN_DEPENDENCY ep.rl 
# )




set_source_files_properties(
	environment.cpp # environ.cpp ep.cpp epv.cpp
	PROPERTIES
	COMPILE_FLAGS 
	"${CMAKE_CXX_FLAGS} -Wno-unused-variable"
)

add_library(MPW_LIB ${MPW_SRC})

target_link_libraries(MPW_LIB MACOS_LIB NATIVE_LIB)
