cmake_minimum_required(VERSION 3.1)
project(mpw)
set (PROJECT_TYPE "CXX")
set (PROJECT_NAME "mpw")

# these are only effective as of CMake 3.1, apparently.
# Use c++14 for all C++ files, c99 for C files.
#
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS FALSE)


if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	add_compile_options(-g -Wall)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

	add_compile_options(-g -Wall -Wno-unknown-pragmas -Wno-multichar)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

#
# create the config.h file.
#

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckStructHasMember)

CHECK_INCLUDE_FILES(endian.h HAVE_ENDIAN_H)
CHECK_INCLUDE_FILES(sys/endian.h HAVE_SYS_ENDIAN_H)
CHECK_INCLUDE_FILES(machine/endian.h HAVE_MACHINE_ENDIAN_H)

CHECK_STRUCT_HAS_MEMBER("struct dirent" d_namlen dirent.h HAVE_DIRENT_D_NAMLEN)
CHECK_STRUCT_HAS_MEMBER("struct stat" st_birthtime sys/stat.h HAVE_STAT_ST_BIRTHTIME)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)


add_subdirectory(bin)
add_subdirectory(cpu)
add_subdirectory(toolbox)
add_subdirectory(mplite)
add_subdirectory(mpw)
add_subdirectory(macos)
add_subdirectory(cxx)
add_subdirectory(native)
