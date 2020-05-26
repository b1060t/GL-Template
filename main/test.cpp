// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//#include <assimp/texture.h>
#include <mango/mango.hpp>

#include <shader.hpp>
#include <element.hpp>

using namespace glm;

static GLfloat b[] = {
    -1.0f,-1.0f,-1.0f,    0.000059f, 1.0f-0.000004f,
	-1.0f,-1.0f, 1.0f,    0.000103f, 1.0f-0.336048f,
	-1.0f, 1.0f, 1.0f,    0.335973f, 1.0f-0.335903f,
	 1.0f, 1.0f,-1.0f,    1.000023f, 1.0f-0.000013f,
	-1.0f,-1.0f,-1.0f,    0.667979f, 1.0f-0.335851f,
	-1.0f, 1.0f,-1.0f,    0.999958f, 1.0f-0.336064f,
	 1.0f,-1.0f, 1.0f,    0.667979f, 1.0f-0.335851f,
	-1.0f,-1.0f,-1.0f,    0.336024f, 1.0f-0.671877f,
	 1.0f,-1.0f,-1.0f,    0.667969f, 1.0f-0.671889f,
	 1.0f, 1.0f,-1.0f,    1.000023f, 1.0f-0.000013f,
	 1.0f,-1.0f,-1.0f,    0.668104f, 1.0f-0.000013f,
	-1.0f,-1.0f,-1.0f,    0.667979f, 1.0f-0.335851f,
	-1.0f,-1.0f,-1.0f,    0.000059f, 1.0f-0.000004f,
	-1.0f, 1.0f, 1.0f,    0.335973f, 1.0f-0.335903f,
	-1.0f, 1.0f,-1.0f,    0.336098f, 1.0f-0.000071f,
	 1.0f,-1.0f, 1.0f,    0.667979f, 1.0f-0.335851f,
	-1.0f,-1.0f, 1.0f,    0.335973f, 1.0f-0.335903f,
	-1.0f,-1.0f,-1.0f,    0.336024f, 1.0f-0.671877f,
	-1.0f, 1.0f, 1.0f,    1.000004f, 1.0f-0.671847f,
	-1.0f,-1.0f, 1.0f,    0.999958f, 1.0f-0.336064f,
	 1.0f,-1.0f, 1.0f,    0.667979f, 1.0f-0.335851f,
	 1.0f, 1.0f, 1.0f,    0.668104f, 1.0f-0.000013f,
	 1.0f,-1.0f,-1.0f,    0.335973f, 1.0f-0.335903f,
	 1.0f, 1.0f,-1.0f,    0.667979f, 1.0f-0.335851f,
	 1.0f,-1.0f,-1.0f,    0.335973f, 1.0f-0.335903f,
	 1.0f, 1.0f, 1.0f,    0.668104f, 1.0f-0.000013f,
	 1.0f,-1.0f, 1.0f,    0.336098f, 1.0f-0.000071f,
	 1.0f, 1.0f, 1.0f,    0.000103f, 1.0f-0.336048f,
	 1.0f, 1.0f,-1.0f,    0.000004f, 1.0f-0.671870f,
	-1.0f, 1.0f,-1.0f,    0.336024f, 1.0f-0.671877f,
	 1.0f, 1.0f, 1.0f,    0.000103f, 1.0f-0.336048f,
	-1.0f, 1.0f,-1.0f,    0.336024f, 1.0f-0.671877f,
	-1.0f, 1.0f, 1.0f,    0.335973f, 1.0f-0.335903f,
	 1.0f, 1.0f, 1.0f,    0.667969f, 1.0f-0.671889f,
	-1.0f, 1.0f, 1.0f,    1.000004f, 1.0f-0.671847f,
	 1.0f,-1.0f, 1.0f,    0.667979f, 1.0f-0.335851f
};

extern const char _binary_shaders_test_vert_start, _binary_shaders_test_vert_end;
extern const char _binary_shaders_test_frag_start, _binary_shaders_test_frag_end;
extern const char _binary_misc_a_tga_start, _binary_misc_a_tga_end;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Test", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::vector<Attr> v;
    Attr a1, a2;
    a1.size=3; a1.norm=GL_FALSE; a1.type=GL_FLOAT; a1.stride=5; a1.offset=0;
    a2.size=2; a2.norm=GL_FALSE; a2.type=GL_FLOAT; a2.stride=5; a2.offset=3;
    v.push_back(a1); v.push_back(a2);

    mango::Memory mem((unsigned char*)(&_binary_misc_a_tga_start), (&_binary_misc_a_tga_end-&_binary_misc_a_tga_start)*sizeof(unsigned char));
    mango::Bitmap m(mem, ".tga", mango::FORMAT_B8G8R8);
    
    Shader test_shader(
        std::string(&_binary_shaders_test_vert_start, &_binary_shaders_test_vert_end - &_binary_shaders_test_vert_start),
        std::string(&_binary_shaders_test_frag_start, &_binary_shaders_test_frag_end - &_binary_shaders_test_frag_start)
    );

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)4/(float)3, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    Element e(b, sizeof(b), v, &m, &test_shader);

	do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw nothing, see you in tutorial 2 !
        e.render(View, Projection);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}