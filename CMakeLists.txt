cmake_minimum_required(VERSION 2.6)
project(mpw)
set (PROJECT_TYPE "CXX")
set (PROJECT_NAME "mpw")

# these are only effective as of CMake 3.1, apparently.
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
	set(CMAKE_C_FLAGS "-Wall  -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
	set(CMAKE_C_COMPILER "gcc")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -g -Wno-unknown-pragmas")
	set(CMAKE_C_FLAGS "-std=c99 -Wall -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(CMAKE_C_COMPILER "gcc")
	set(CMAKE_CXX_COMPILER "g++")
	set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -g -Wno-unknown-pragmas")
	set(CMAKE_C_FLAGS "-std=c99 -Wall -g")
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")

endif()


add_subdirectory(bin)
add_subdirectory(cpu)
add_subdirectory(toolbox)
add_subdirectory(mplite)
add_subdirectory(mpw)
add_subdirectory(macos)
add_subdirectory(native)
