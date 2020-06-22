// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

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

EXTLD(shaders_geotest_vert);
EXTLD(shaders_geotest_frag);
EXTLD(shaders_geotest_geom);
EXTLD(shaders_circle_vert);
EXTLD(shaders_circle_frag);
EXTLD(shaders_circle_geom);

const GLfloat vertices[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 1.0f, 1.0f, 0.0f
};

const GLfloat circles[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.2f,
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.1f,
	 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.4f,
	 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.3f
};

const GLint indices[] = {
	0,1,2,3
};

int main()
{
	Context context("Geometry");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	Shader geo_shader(
		std::string(LDVAR(shaders_geotest_vert), LDLEN(shaders_geotest_vert)),
		std::string(LDVAR(shaders_geotest_geom), LDLEN(shaders_geotest_geom)),
	    std::string(LDVAR(shaders_geotest_frag), LDLEN(shaders_geotest_frag))
	);

	Shader circle_shader(
		std::string(LDVAR(shaders_circle_vert), LDLEN(shaders_circle_vert)),
		std::string(LDVAR(shaders_circle_geom), LDLEN(shaders_circle_geom)),
	    std::string(LDVAR(shaders_circle_frag), LDLEN(shaders_circle_frag))
	);

	Element e(const_cast<GLfloat*>(&vertices[0]), const_cast<GLint*>(&indices[0]), sizeof(GLfloat)*20, sizeof(GLint)*4, TWOD_COLORED_POINT_ATTR);
	e.attachShader(&geo_shader);
	std::vector<Attr> CIRCLE_ATTR = {
        {2, GL_FLOAT, GL_FALSE, 6, 0}, //position x,y
        {3, GL_FLOAT, GL_FALSE, 6, 2}, //color r,g,b
		{1, GL_FLOAT, GL_FALSE, 6, 5}, //radius
    };
	Element c(const_cast<GLfloat*>(&circles[0]), const_cast<GLint*>(&indices[0]), sizeof(circles), sizeof(indices), CIRCLE_ATTR);
	c.attachShader(&circle_shader);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

		e.render(GL_POINTS);
		c.render(GL_POINTS);

		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));
}