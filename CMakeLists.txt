cmake_minimum_required (VERSION 3.3)
project(glTemplate)
include(CMakeOptions.txt)

find_package(OpenGL REQUIRED)

add_custom_target(rscembedding
    DEPENDS ${PROJECT_BINARY_DIR}/bin2c)
ADD_CUSTOM_COMMAND(OUTPUT ${PROJECT_BINARY_DIR}/bin2c
  COMMAND gcc -o ${PROJECT_BINARY_DIR}/bin2c external/bin2c/bin2c.c
  DEPENDS external/bin2c/bin2c.c
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Building bin2c"
  VERBATIM
)

set(CMAKE_CXX_STANDARD 14)

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

if(NOT TIDE_EXCLUDE)
    include_directories(
        tide/
    )
endif()

include_directories(
    external/glfw/include/
    external/glew/include/
    external/glm
    external/assimp/include/
    external/mango/include/
) 

if(APPLE)
    set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        assimp
        mango-opengl
    )
else()
    set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        assimp
        mango-opengl
        -static-libgcc
        -static-libstdc++
    )
endif(APPLE)

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
    if(NOT APPLE)
        SET(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
        ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
          COMMAND ld -r -b binary -o ${out_f} ${src_f}
          DEPENDS ${in_f}
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          COMMENT "Building object ${out_f}"
          VERBATIM
          )
    else()
        SET(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
        SET(c_f "${PROJECT_BINARY_DIR}/${in_f}.c")
        string(REPLACE "/" "_" tmp ${in_f})
        string(REPLACE "." "_" out_name ${tmp})
        ADD_CUSTOM_COMMAND(OUTPUT ${c_f}
          COMMAND ${PROJECT_BINARY_DIR}/bin2c ${src_f} ${c_f} ${out_name}
          DEPENDS ${in_f}
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          COMMENT "Building object ${c_f}"
          VERBATIM
          )
        ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
          COMMAND gcc -c ${c_f} -o ${out_f}
          DEPENDS ${c_f}
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          COMMENT "Building object ${out_f}"
          VERBATIM
          )
    endif()
    LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/shaders)
FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/misc)

find_package(Threads)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include(CMakeExecutables.txt)