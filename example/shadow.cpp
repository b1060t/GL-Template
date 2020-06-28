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

EXTLD(shaders_plane_vert);
EXTLD(shaders_plane_frag);
EXTLD(misc_wood_png);
EXTLD(shaders_skybox_vert);
EXTLD(shaders_skybox_frag);
EXTLD(misc_skybox_back_jpg);
EXTLD(misc_skybox_bottom_jpg);
EXTLD(misc_skybox_front_jpg);
EXTLD(misc_skybox_left_jpg);
EXTLD(misc_skybox_right_jpg);
EXTLD(misc_skybox_top_jpg);

int main()
{
	Context context("Shadow");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	Camera cam(context.getWindow(), WIDTH, HEIGHT, 45.0f, glm::vec3(-2,10,0));
	glfwSetCursorPos(context.getWindow(), WIDTH/2, HEIGHT/2);

	ResourcePool& rsc = ResourcePool::getResourcePool();
	rsc.loadTexture("img", LDVAR(misc_wood_png), LDLEN(misc_wood_png), ".png");
	mango::Memory back_mem((unsigned char*)LDVAR(misc_skybox_back_jpg), LDLEN(misc_skybox_back_jpg));
	mango::Bitmap back_tex(back_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory bottom_mem((unsigned char*)LDVAR(misc_skybox_bottom_jpg), LDLEN(misc_skybox_bottom_jpg));
	mango::Bitmap bottom_tex(bottom_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory front_mem((unsigned char*)LDVAR(misc_skybox_front_jpg), LDLEN(misc_skybox_front_jpg));
	mango::Bitmap front_tex(front_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory left_mem((unsigned char*)LDVAR(misc_skybox_left_jpg), LDLEN(misc_skybox_left_jpg));
	mango::Bitmap left_tex(left_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory right_mem((unsigned char*)LDVAR(misc_skybox_right_jpg), LDLEN(misc_skybox_right_jpg));
	mango::Bitmap right_tex(right_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory top_mem((unsigned char*)LDVAR(misc_skybox_top_jpg), LDLEN(misc_skybox_top_jpg));
	mango::Bitmap top_tex(top_mem, ".jpg", mango::FORMAT_B8G8R8);
	GLuint skyboxtex;
	glGenTextures(1, &skyboxtex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxtex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, right_tex.width, right_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, right_tex.image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, left_tex.width, left_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, left_tex.image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, top_tex.width, top_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, top_tex.image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, bottom_tex.width, bottom_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom_tex.image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, front_tex.width, front_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, front_tex.image);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, back_tex.width, back_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, back_tex.image);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	rsc.loadShader(
		"skybox",
		std::string(LDVAR(shaders_skybox_vert), LDLEN(shaders_skybox_vert)),
		std::string(LDVAR(shaders_skybox_frag), LDLEN(shaders_skybox_frag))
	);
	rsc.loadShader(
		"tex",
		std::string(LDVAR(shaders_plane_vert), LDLEN(shaders_plane_vert)),
		std::string(LDVAR(shaders_plane_frag), LDLEN(shaders_plane_frag))
	);

	Element plane(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), sizeof(ImageVertices), sizeof(ImageIndices), TWOD_TEXTURE_ATTR);
	plane.attachShader(rsc.shaderdic["tex"]);
	plane.texdic["material.diffuse"] = rsc.texdic["img"]->handle;
	plane.vec3dic["material.specular"] = glm::vec3(1.0f, 1.0f, 1.0f);
	plane.floatdic["material.shiness"] = 32.0f;
	plane.vec3dic["light.diffuse"] = glm::vec3(0.7f);
	plane.vec3dic["light.ambient"] = glm::vec3(0.2f);
	plane.vec3dic["light.specular"] = glm::vec3(1.0f);
	plane.vec3dic["light.pos"]=glm::vec3(0.2f,5.0f,0.0f);
	plane.internal_model = true;
	plane.setPosition(glm::vec3(0.0f,0.0f,0.0f));
	plane.setScale(glm::vec3(50.0f));
	plane.setRotation(glm::vec3(0.0f,0.0f,0.0f));

	Element skybox(const_cast<GLfloat*>(&SkyboxVertices[0]), const_cast<GLint*>(&SkyboxIndices[0]), sizeof(SkyboxVertices), sizeof(SkyboxIndices), SKYBOX_ATTR);
	skybox.attachShader(rsc.shaderdic["skybox"]);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		cam.loop();

		skybox.mat4dic["View"] = glm::mat4(glm::mat3(cam.view)); // skybox remove translation
		skybox.mat4dic["Projection"] = cam.projection;
		plane.mat4dic["View"]=cam.view;
		plane.mat4dic["Projection"]=cam.projection;
		plane.vec3dic["viewPos"]=cam.getPos();
		skybox.render();
		plane.render();

		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}