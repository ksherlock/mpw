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

if (APPLE)
	set(CMAKE_C_COMPILER "clang")
	set(CMAKE_CXX_COMPILER "clang++")
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ -Wall -g")
	set(CMAKE_C_FLAGS "-Wall -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
	set(CMAKE_C_COMPILER "gcc")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-Wall -g -Wno-unknown-pragmas")
	set(CMAKE_C_FLAGS "-std=c99 -Wall -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(CMAKE_C_COMPILER "gcc")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-Wall -g -Wno-unknown-pragmas")
	set(CMAKE_C_FLAGS "-Wall -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")

endif()

#
# create the config.h file.
#

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckTypeSize)

CHECK_INCLUDE_FILES(endian.h HAVE_ENDIAN_H)
CHECK_INCLUDE_FILES(sys/endian.h HAVE_SYS_ENDIAN_H)
CHECK_INCLUDE_FILES(machine/endian.h HAVE_MACHINE_ENDIAN_H)

SET(CMAKE_EXTRA_INCLUDE_FILES dirent.h)
CHECK_TYPE_SIZE("((struct dirent *)0)->d_namlen" DIRENT_D_NAMLEN)
SET(CMAKE_EXTRA_INCLUDE_FILES)

SET(CMAKE_EXTRA_INCLUDE_FILES sys/stat.h)
CHECK_TYPE_SIZE("((struct stat *)0)->st_birthtime" STAT_ST_BIRTHTIME)
SET(CMAKE_EXTRA_INCLUDE_FILES)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)


add_subdirectory(bin)
add_subdirectory(cpu)
add_subdirectory(toolbox)
add_subdirectory(mplite)
add_subdirectory(mpw)
add_subdirectory(macos)
add_subdirectory(cxx)
add_subdirectory(native)
