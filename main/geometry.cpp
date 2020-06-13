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
#include <camera.hpp>
#include <frame.hpp>
#include <common.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

using namespace glm;
using namespace tide;

extern "C" {
	extern const char _binary_shaders_geotest_vert_start, _binary_shaders_geotest_vert_end;
	extern const char _binary_shaders_geotest_frag_start, _binary_shaders_geotest_frag_end;
	extern const char _binary_shaders_geotest_geom_start, _binary_shaders_geotest_geom_end;
}

const GLfloat vertices[] = {
	-0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	0.5f, -0.5f
};

const GLint indices[] = {
	0,1,2,3
};

int main()
{
	Context context("Geometry");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	Shader geo_shader(
		std::string(&_binary_shaders_geotest_vert_start, &_binary_shaders_geotest_vert_end - &_binary_shaders_geotest_vert_start),
		std::string(&_binary_shaders_geotest_geom_start, &_binary_shaders_geotest_geom_end - &_binary_shaders_geotest_geom_start),
	    std::string(&_binary_shaders_geotest_frag_start, &_binary_shaders_geotest_frag_end - &_binary_shaders_geotest_frag_start)
	);

	Element e(const_cast<GLfloat*>(&vertices[0]), const_cast<GLint*>(&indices[0]), sizeof(GLfloat)*8, sizeof(GLint)*4, TWOD_POINT_ATTR);
	e.attachShader(&geo_shader);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

		e.render(GL_POINTS);

		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));
}