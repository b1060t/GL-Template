cmake_minimum_required (VERSION 3.0)
project(glTemplate)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
    common/
    external/glfw/include/
    external/glew/include/
	external/glm
	external/assimp/include/
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

# Compilation step for static resources
FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
    FILE(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
    SET(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
    ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
      COMMAND ld -r -b binary -o ${out_f} ${src_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building GLSL object ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/shaders)
ADD_RESOURCES(RES
    shaders/test.vert
    shaders/test.frag
)

add_executable(a 
    main/test.cpp
    common/shader.cpp
    common/shader.hpp
    ${RES}
)

target_link_libraries(a
	${ALL_LIBS}
)