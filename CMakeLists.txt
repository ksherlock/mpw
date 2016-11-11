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
	add_compile_options(/wd4996 /wd4068)
	include_directories(winclude)
	set(HAVE_ENDIAN_H 1)
endif()


if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	add_compile_options(-D _BSD_SOURCE)
ENDIF()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

#
# create the config.h file.
#

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckStructHasMember)
INCLUDE (CheckFunctionExists)
INCLUDE (CheckLibraryExists)

CHECK_INCLUDE_FILES(endian.h HAVE_ENDIAN_H)
CHECK_INCLUDE_FILES(sys/endian.h HAVE_SYS_ENDIAN_H)
CHECK_INCLUDE_FILES(machine/endian.h HAVE_MACHINE_ENDIAN_H)
CHECK_INCLUDE_FILES(bsd/string.h HAVE_BSD_STRING_H)

CHECK_STRUCT_HAS_MEMBER("struct dirent" d_namlen dirent.h HAVE_DIRENT_D_NAMLEN)
CHECK_STRUCT_HAS_MEMBER("struct stat" st_birthtime sys/stat.h HAVE_STAT_ST_BIRTHTIME)

set(CMAKE_REQUIRED_INCLUDES string.h)
CHECK_FUNCTION_EXISTS(strlcat HAVE_STRLCAT)
CHECK_FUNCTION_EXISTS(strlcpy HAVE_STRLCPY)
set(CMAKE_REQUIRED_INCLUDES)



CHECK_LIBRARY_EXISTS(edit readline "" HAVE_LIBEDIT)
CHECK_LIBRARY_EXISTS(readline readline "" HAVE_LIBREADLINE)
CHECK_LIBRARY_EXISTS(history add_history "" HAVE_LIBHISTORY)

# feature config
SET(ENABLE_DEBUGGER 1 CACHE BOOL "Define whether debugger should be enabled.")
IF(ENABLE_DEBUGGER)
ENDIF()

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)


add_subdirectory(bin)
add_subdirectory(cpu)
add_subdirectory(toolbox)
add_subdirectory(mplite)
add_subdirectory(mpw)
add_subdirectory(macos)
add_subdirectory(cxx)
add_subdirectory(native)
