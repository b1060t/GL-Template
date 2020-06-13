cmake_minimum_required (VERSION 3.3)
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
if(NOT MSVC)
add_definitions(-D_WIN32_WINNT=${ver})
endif(NOT MSVC)

add_subdirectory(external)

include_directories(
    tide/
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

find_package(Threads)

ADD_RESOURCES(A_RES
    shaders/outline.frag
    shaders/shade.vert
    shaders/shade.frag
    shaders/tex.vert
    shaders/tex.frag
    misc/a.obj
    misc/diffuse.jpg
    misc/specular.jpg
    misc/normal.png
)
add_executable(a 
    main/test.cpp
    ${CPP_FILES}
    ${A_RES}
)
target_link_libraries(a
  ${ALL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
)

ADD_RESOURCES(GEOMETRY_RES
    shaders/geotest.vert
    shaders/geotest.geom
    shaders/geotest.frag
)
add_executable(geometry 
    main/geometry.cpp
    ${GEOMETRY_RES}
)
target_link_libraries(geometry
  ${ALL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
)

ADD_RESOURCES(POSTPROCESS_RES
    shaders/tex.vert
    shaders/tex.frag
    misc/postprocess.jpg
    misc/guided.png
    shaders/mean.frag
    shaders/corr.frag
    shaders/alpha.frag
    shaders/beta.frag
    shaders/guided.frag
)
add_executable(postprocess 
    main/postprocess.cpp
    ${POSTPROCESS_RES}
)
target_link_libraries(postprocess
  ${ALL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
)

ADD_RESOURCES(NORMAL_RES
    shaders/shade.vert
    shaders/shade.frag
    shaders/normal.vert
    shaders/normal.geom
    shaders/normal.frag
    misc/a.obj
    misc/diffuse.jpg
    misc/specular.jpg
    misc/normal.png
)
add_executable(normal 
    main/normalvisualize.cpp
    ${CPP_FILES}
    ${NORMAL_RES}
)
target_link_libraries(normal
  ${ALL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
)