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

#include <mango/mango.hpp>

#include <shader.hpp>
#include <element.hpp>
#include <camera.hpp>
#include <context.hpp>
#include <common.hpp>
#include <frame.hpp>

using namespace glm;
using namespace tide;

#define WIDTH 1024
#define HEIGHT 768

EXTLD(shaders_tex_vert);
EXTLD(shaders_tex_frag);
EXTLD(misc_wood_png);

int main()
{
	Context context("Shadow");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	Camera cam(context.getWindow(), WIDTH, HEIGHT, 45.0f, glm::vec3(0,0,0));
	glfwSetCursorPos(context.getWindow(), WIDTH/2, HEIGHT/2);

	mango::Memory img_mem((unsigned char*)LDVAR(misc_wood_png), LDLEN(misc_wood_png));
	mango::Bitmap img_tex(img_mem, ".png", mango::FORMAT_B8G8R8);
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_tex.width, img_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Shader tex_shader(
	    std::string(LDVAR(shaders_tex_vert), LDLEN(shaders_tex_vert)),
	    std::string(LDVAR(shaders_tex_frag), LDLEN(shaders_tex_frag))
	);

	Element plane(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), sizeof(ImageVertices), sizeof(ImageIndices), TWOD_TEXTURE_ATTR);
	plane.attachShader(&tex_shader);
	plane.texdic["tex"] = tex;
	plane.texdic["width"] = img_tex.width;
	plane.texdic["height"] = img_tex.height;
	plane.internal_model = false;

	do
	{
		cam.loop();

		plane.render();
        
		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}