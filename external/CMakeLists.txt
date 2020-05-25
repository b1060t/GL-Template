add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_subdirectory(glfw)
include_directories(
    glfw/include/
    glew/include/
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


set(GLEW_SOURCE glew/src/glew.c)
add_library( GLEW STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)
target_link_libraries(GLEW
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)


include_directories(assimp/code)
include_directories(assimp/contrib/zlib)
include_directories(assimp/contrib/irrXML)
if (MSVC)
	add_compile_options(/bigobj)
elseif(CMAKE_COMPILER_IS_MINGW)
	add_compile_options(-Wa,-mbig-obj)
endif()
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/assimp/include/assimp/config.h
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h
)
SET (ASSIMP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
SET (ASSIMP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
SET (ASSIMP_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET (ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
SET (ASSIMP_SOVERSION 5)
SET( ASSIMP_PACKAGE_VERSION "0" CACHE STRING "the package-specific version used for uploading the sources" )
IF(NOT IGNORE_GIT_HASH)
  # Get the current working branch
  EXECUTE_PROCESS(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
  # Get the latest abbreviated commit hash of the working branch
  EXECUTE_PROCESS(
    COMMAND git rev-parse --short=8 HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
ENDIF()
IF(NOT GIT_COMMIT_HASH)
  SET(GIT_COMMIT_HASH 0)
ENDIF(NOT GIT_COMMIT_HASH)

IF(ASSIMP_DOUBLE_PRECISION)
    ADD_DEFINITIONS(-DASSIMP_DOUBLE_PRECISION)
ENDIF(ASSIMP_DOUBLE_PRECISION)
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/revision.h.in
	${CMAKE_CURRENT_BINARY_DIR}/assimp/code/common/revision.h
)
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/revision.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/revision.h
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h.included
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h
)

set(ASSIMP_LIB_INSTALL_DIR "lib")
set(LIBASSIMP_COMPONENT libassimp)
set(ZLIB_LIBRARIES zlib)
set(BUILD_STATIC_LIB ON)
set(ASSIMP_WERROR OFF)

add_subdirectory(assimp/code)


ADD_DEFINITIONS(-fpermissive)
include_directories(
    mango/include/
    mango/source/external/libwebp/
)
add_subdirectory(mango/build)