cmake_minimum_required (VERSION 3.0)
project(glTemplate)

find_package(OpenGL REQUIRED)

macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()
get_WIN32_WINNT(ver)
add_definitions(-D_WIN32_WINNT=${ver})

add_subdirectory(external)

include_directories(
    common/
    external/glfw/include/
    external/glew/include/
    external/glm
    external/assimp/include/
    external/mango/include/
) 

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
    GLEW
    assimp
    mango-opengl
    -static-libgcc
    -static-libstdc++
)

set(CPP_FILES
    common/shader.cpp
    common/element.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_EXE_LINKER_FLAGS " -static")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

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
      COMMENT "Building object ${out_f}"
      VERBATIM
      )
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/shaders)
FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/misc)
ADD_RESOURCES(RES
    shaders/test.vert
    shaders/test.frag
    shaders/shade.vert
    shaders/shade.frag
    misc/a.obj
    misc/diffuse.jpg
    misc/specular.jpg
    misc/normal.png
)

find_package(Threads)
add_executable(a 
    main/test.cpp
    ${CPP_FILES}
    ${RES}
)

target_link_libraries(a
  ${ALL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
)