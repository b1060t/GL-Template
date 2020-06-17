#glfw
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

#assimp
set(ZLIB_LIBRARIES zlib)
set(BUILD_STATIC_LIB ON CACHE BOOL "assimp static lib")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "disable assimp tools")
set(ASSIMP_WERROR OFF CACHE BOOL "disable assimp werror")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "disable assimp tests")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "assimp shared lib")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "disable all export")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "disable all importers")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "enable obj importer")
if(MSVC)
#set(CMAKE_CXX_FLAGS "/O3")
else()
set(CMAKE_CXX_FLAGS "-O3")
endif(MSVC)
add_subdirectory(assimp)

#mango
if(MSVC)
ADD_DEFINITIONS(/permissive)
else()
ADD_DEFINITIONS(-fpermissive)
endif(MSVC)
include_directories(
    mango/include/
    mango/source/external/libwebp/
)
add_subdirectory(mango/build)