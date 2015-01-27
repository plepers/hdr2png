cmake_minimum_required (VERSION 2.6)


project (hdr2png)


set( PROJECT_SOURCES
	main.cpp
	main.h
	
	hdrloader.cpp
	lodepng.cpp

	hdrloader.h
	lodepng.h
)

include_directories("${PROJECT_SOURCE_DIR}/submodules/tclap/include")

add_executable(hdr2png ${PROJECT_SOURCES})