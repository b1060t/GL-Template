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
add_compile_options(-Wa,-mbig-obj)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/include/assimp/config.h
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h.included
	${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h
)
#add_definitions(-Wno-error)
#add_subdirectory(assimp/contrib/zlib)

set(LIB_INSTALL_DIR "lib")
set(LIBASSIMP_COMPONENT libassimp)
set(ZLIB_LIBRARIES zlib)
set(BUILD_STATIC_LIB ON)
set(ASSIMP_WERROR OFF)

add_subdirectory(assimp/code)