cmake_minimum_required(VERSION 3.1)
project(mpw VERSION 0.8.3 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS FALSE)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        # Clang or AppleClang
        set(CMAKE_CXX_FLAGS "-Wall")
endif()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
	# Clang or AppleClang
	set(CMAKE_C_FLAGS "-Wall")
endif()


find_program(RAGEL NAMES ragel)
if (NOT RAGEL)
	message(FATAL_ERROR "ragel (version 6) is required.")
endif()


include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(bin)
add_subdirectory(cpu)
add_subdirectory(toolbox)
add_subdirectory(mplite)
add_subdirectory(mpw)
add_subdirectory(macos)

add_subdirectory(libsane)
