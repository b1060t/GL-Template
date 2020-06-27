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

EXTLD(shaders_defog_vert);
EXTLD(shaders_defog_frag);
EXTLD(shaders_tex_vert);
EXTLD(shaders_tex_frag);
EXTLD(misc_defog_jpg);

int main()
{
	mango::Memory img_mem((unsigned char*)LDVAR(misc_defog_jpg), LDLEN(misc_defog_jpg));
	mango::Bitmap img_tex(img_mem, ".jpg", mango::FORMAT_B8G8R8);

	Context context("Defog", img_tex.width, img_tex.height);
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	GLuint img;
	glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_tex.width, img_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Shader defog_shader(
		std::string(LDVAR(shaders_defog_vert), LDLEN(shaders_defog_vert)),
		std::string(LDVAR(shaders_defog_frag), LDLEN(shaders_defog_frag))
	);

	Shader tex_shader(
		std::string(LDVAR(shaders_tex_vert), LDLEN(shaders_tex_vert)),
		std::string(LDVAR(shaders_tex_frag), LDLEN(shaders_tex_frag))
	);

	Frame fbo(img_tex.width, img_tex.height, FRAME_ATTACHMENTS_2COLOR);

	tide::Element guided(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
	guided.attachShader(&defog_shader);
	guided.setClearFlag(GL_COLOR_BUFFER_BIT);

	Frame fbo1(img_tex.width, img_tex.height, FRAME_ATTACHMENTS_2COLOR);
	guided.texdic["I"]=img;
	guided.floatdic["width"]=img_tex.width;
	guided.floatdic["height"]=img_tex.height;
	guided.intdic["phase"]=2;
	fbo1.bind();
	guided.render();
	fbo1.unbind();

	Frame fbo2(img_tex.width, img_tex.height, FRAME_ATTACHMENTS_2COLOR);
	guided.texdic["I"]=fbo1.getTex(0);
	guided.texdic["P"]=img;
	guided.internal_model = false;
	guided.intdic["phase"]=0;
	fbo2.bind();
	guided.render();
	fbo2.unbind();

	Frame fbo3(img_tex.width, img_tex.height, FRAME_ATTACHMENTS_2COLOR);
	guided.texdic["a"]=fbo2.getTex(0);
	guided.texdic["b"]=fbo2.getTex(1);
	guided.texdic["I"]=fbo1.getTex(0);
	guided.intdic["phase"]=1;
	fbo3.bind();
	guided.render();
	fbo3.unbind();

	guided.texdic["I"]=fbo3.getTex(0);
	guided.texdic["P"]=img;
	guided.internal_model = false;
	guided.intdic["phase"]=3;

	do
	{
		guided.render();
		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}