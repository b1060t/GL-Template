// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
//GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//#include <assimp/texture.h>
#include <mango/mango.hpp>

#include <shader.hpp>
#include <element.hpp>
#include <camera.hpp>
#include <context.hpp>
#include <common.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#define WIDTH 1024
#define HEIGHT 768

using namespace glm;
using namespace tide;

EXTLD(shaders_shade_vert);
EXTLD(shaders_shade_frag);
EXTLD(shaders_normal_vert);
EXTLD(shaders_normal_geom);
EXTLD(shaders_normal_frag);
EXTLD(misc_diffuse_jpg);
EXTLD(misc_specular_jpg);
EXTLD(misc_normal_png);
EXTLD(misc_a_obj);

int main()
{
	Context context("Normal");

	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	mango::Memory diffuse_mem((unsigned char*)LDVAR(misc_diffuse_jpg), LDLEN(misc_diffuse_jpg));
	mango::Bitmap diffuse_tex(diffuse_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory specular_mem((unsigned char*)LDVAR(misc_specular_jpg), LDLEN(misc_specular_jpg));
	mango::Bitmap specular_tex(specular_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory normal_mem((unsigned char*)LDVAR(misc_normal_png), LDLEN(misc_normal_png));
	mango::Bitmap normal_tex(normal_mem, ".png", mango::FORMAT_B8G8R8);
	GLuint diffuse, specular, normal;
	glGenTextures(1, &diffuse);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, diffuse_tex.width, diffuse_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, diffuse_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenTextures(1, &specular);
	glBindTexture(GL_TEXTURE_2D, specular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, specular_tex.width, specular_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, specular_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenTextures(1, &normal);
	glBindTexture(GL_TEXTURE_2D, normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normal_tex.width, normal_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, normal_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	Shader shade_shader(
	    std::string(LDVAR(shaders_shade_vert), LDLEN(shaders_shade_vert)),
	    std::string(LDVAR(shaders_shade_frag), LDLEN(shaders_shade_frag))
	);
    Shader normal_shader(
		std::string(LDVAR(shaders_normal_vert), LDLEN(shaders_normal_vert)),
		std::string(LDVAR(shaders_normal_geom), LDLEN(shaders_normal_geom)),
	    std::string(LDVAR(shaders_normal_frag), LDLEN(shaders_normal_frag))
	);

	Assimp::Importer importer;
	const aiScene* sceneObjPtr = importer.ReadFileFromMemory((void*)LDVAR(misc_a_obj), LDLEN(misc_a_obj),aiProcess_ValidateDataStructure);
	Element e(sceneObjPtr, tide::THREED_OBJECT_ATTR);
	
	e.addTexture("material.diffuse", diffuse);
	e.addTexture("material.specular", specular);
	e.addTexture("material.normal", normal);
	e.addFloatUniform("material.shiness", 32.0f);
	e.addVec3Uniform("light.pos", glm::vec3(1.0f,0.0f,0.0f));
	e.addVec3Uniform("light.diffuse", glm::vec3(0.7f));
	e.addVec3Uniform("light.ambient", glm::vec3(0.2f));
	e.addVec3Uniform("light.specular", glm::vec3(1.0f));
	e.internal_model = true;
    e.setPosition(glm::vec3(-0.1f,-0.2f,-0.3f));
    e.setScale(glm::vec3(0.8f));
    e.setRotation(glm::vec3(0.0f,0.0f,0.0f));

	Camera cam(context.getWindow(), WIDTH, HEIGHT, 45.0f, glm::vec3(0,0,4));
    glfwSetCursorPos(context.getWindow(), WIDTH/2, HEIGHT/2);

	glEnable(GL_DEPTH_TEST);

	do
	{
		cam.loop();

		float x = 1.0f * sin(glfwGetTime());
		float z = 1.0f * cos(glfwGetTime());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		e.vec3dic["light.pos"]=glm::vec3(x,0.0f,z);
		e.mat4dic["View"]=cam.view;
		e.mat4dic["Projection"]=cam.projection;
		e.vec3dic["viewPos"]=cam.getPos();
		e.attachShader(&shade_shader);
		e.render();
		e.attachShader(&normal_shader);
		e.render();

		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}