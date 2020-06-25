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


ADD_DEFINITIONS(-DGLEW_STATIC)
set(GLEW_SRC
	glew/src/glew.c
	glew/src/glewinfo.c
	glew/src/visualinfo.c
)
set(GLEW_INC
	glew/include/GL/eglew.h
	glew/include/GL/glew.h
	glew/include/GL/glxew.h
	glew/include/GL/wglew.h
)
add_library(glew STATIC
	${GLEW_SRC}
	${GLEW_INC}
)
target_link_libraries(glew
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

#assimp
if(TIDE_ASSIMP)
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
endif(TIDE_ASSIMP)

#mango
if(TIDE_MANGO)
	if(MSVC)
	ADD_DEFINITIONS(/permissive)
	else()
	ADD_DEFINITIONS(-fpermissive)
	endif(MSVC)
	add_subdirectory(mango/build)
endif(TIDE_MANGO)

#imgui
if(TIDE_IMGUI)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(GLEW_INC_PATH
	glew/include/GL
)
set(GLFW_INC_PATH
	glfw/include
	glfw/deps
)
set(IMGUI_INC
	${IMGUI_DIR}
	${IMGUI_DIR}/examples/
	${IMGUI_DIR}/examples/example_glfw_opengl3
	${GLEW_INC_PATH}
	${GLFW_INC_PATH}
	${OPENGL_INCLUDE_DIRS}
)
set(IMGUI_SRC
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/examples/imgui_impl_glfw.cpp
	${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_INC})
target_include_directories(imgui PUBLIC
	${IMGUI_DIR}
	${IMGUI_DIR}/examples/
	${IMGUI_DIR}/examples/example_glfw_opengl3
	${GLEW_INC_PATH}
	${GLFW_INC_PATH}
	${OPENGL_INCLUDE_DIRS}
)
#target_link_libraries(imgui PUBLIC glew glfw ${OPENGL_LIBRARY} ${EXTRA_LIBS})
endif(TIDE_IMGUI)
