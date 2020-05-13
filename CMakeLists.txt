cmake_minimum_required (VERSION 3.0)
project(glTemplate)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
    common/
    external/glfw/include/
    external/glew/include/
    external/glm
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(a 
	main/test.cpp
)
target_link_libraries(a
	${ALL_LIBS}
)