
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(TOOLBOX_SRC 
	toolbox.cpp
	mm.cpp
	loader.cpp
	rm.cpp
	os.cpp
	os_alias.cpp
	os_fileinfo.cpp
	os_gestalt.cpp
	os_hfs_dispatch.cpp
	os_highlevel.cpp
	os_hwpriv.cpp
	os_internal.cpp
	os_volume.cpp
	qd.cpp
	sane.cpp
	packages.cpp
	pathnames.cpp
	process.cpp
	utility.cpp
	fs_spec.cpp
	realpath.c
	dispatch.cpp
	fpinfo.cpp
	debug.cpp

)

add_custom_command(
	OUTPUT pathnames.cpp
	COMMAND "${RAGEL}" -p -G2 -o pathnames.cpp "${CMAKE_CURRENT_SOURCE_DIR}/pathnames.rl"
	MAIN_DEPENDENCY pathnames.rl 
)

set_source_files_properties(
	pathnames.cpp
	PROPERTIES
	COMPILE_FLAGS 
	"${CMAKE_CXX_FLAGS} -Wno-unused-variable"
)


add_library(TOOLBOX_LIB ${TOOLBOX_SRC})
target_link_libraries(TOOLBOX_LIB MACOS_LIB sane)
