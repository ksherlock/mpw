set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -Wno-deprecated-declarations -g")

add_definitions(-I ${CMAKE_SOURCE_DIR}/)

set(TOOLBOX_SRC 
	toolbox.cpp
	rm.cpp
	mm.cpp
	os.cpp
	qd.cpp
	sane.cpp
)



add_library(TOOLBOX_LIB ${TOOLBOX_SRC})