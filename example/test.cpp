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

using namespace glm;
using namespace tide;

EXTLD(shaders_shade_vert);
EXTLD(shaders_shade_frag);
EXTLD(shaders_tex_vert);
EXTLD(shaders_tex_frag);
EXTLD(shaders_outline_frag);
EXTLD(misc_diffuse_jpg);
EXTLD(misc_specular_jpg);
EXTLD(misc_normal_png);
EXTLD(misc_a_obj);

GLfloat vertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};
GLint indices[] = {
    0, 1, 2,
    0, 2, 3
};

#define WIDTH 1024
#define HEIGHT 768

int main( void )
{
	Context context("Test");

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
    
	//Shader test_shader(
	//    std::string(&_binary_shaders_test_vert_start, &_binary_shaders_test_vert_end - &_binary_shaders_test_vert_start),
	//    std::string(&_binary_shaders_test_frag_start, &_binary_shaders_test_frag_end - &_binary_shaders_test_frag_start)
	//);

	Shader shade_shader(
	    std::string(LDVAR(shaders_shade_vert), LDLEN(shaders_shade_vert)),
	    std::string(LDVAR(shaders_shade_frag), LDLEN(shaders_shade_frag))
	);

    Shader tex_shader(
	    std::string(LDVAR(shaders_tex_vert), LDLEN(shaders_tex_vert)),
	    std::string(LDVAR(shaders_tex_frag), LDLEN(shaders_tex_frag))
	);

    Shader outline_shader(
        std::string(LDVAR(shaders_shade_vert), LDLEN(shaders_shade_vert)),
	    std::string(LDVAR(shaders_outline_frag), LDLEN(shaders_outline_frag))
    );


	Assimp::Importer importer;
	//const aiScene* sceneObjPtr = importer.ReadFile("a.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* sceneObjPtr = importer.ReadFileFromMemory((void*)LDVAR(misc_a_obj), LDLEN(misc_a_obj),aiProcess_ValidateDataStructure);
	Element e(sceneObjPtr, tide::THREED_OBJECT_ATTR);
	e.attachShader(&shade_shader);
	e.addTexture("material.diffuse", diffuse);
	e.addTexture("material.specular", specular);
	e.addTexture("material.normal", normal);
	e.addFloatUniform("material.shiness", 32.0f);
	e.addVec3Uniform("light.diffuse", glm::vec3(0.7f));
	e.addVec3Uniform("light.ambient", glm::vec3(0.2f));
	e.addVec3Uniform("light.specular", glm::vec3(1.0f));

    Element o(sceneObjPtr, tide::THREED_OBJECT_ATTR);
	o.attachShader(&outline_shader);
	o.addTexture("material.diffuse", diffuse);
	o.addTexture("material.specular", specular);
	o.addTexture("material.normal", normal);
	o.addFloatUniform("material.shiness", 32.0f);
	o.addVec3Uniform("light.diffuse", glm::vec3(0.7f));
	o.addVec3Uniform("light.ambient", glm::vec3(0.2f));
	o.addVec3Uniform("light.specular", glm::vec3(1.0f));

    Element t(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&indices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    t.attachShader(&tex_shader);
    t.addTexture("texture", GLuint(0));
    t.addFloatUniform("width", WIDTH);
    t.addFloatUniform("height", HEIGHT);
    t.internal_model = false;

    e.internal_model = true;
    e.setPosition(glm::vec3(-0.1f,-0.2f,-0.3f));
    e.setScale(glm::vec3(0.8f));
    e.setRotation(glm::vec3(0.0f,0.0f,0.0f));

    o.internal_model = true;
    o.setPosition(glm::vec3(-0.1f,-0.2f,-0.3f));
    o.setScale(glm::vec3(0.82f));
    o.setRotation(glm::vec3(0.0f,0.0f,0.0f));

	Camera cam(context.getWindow(), WIDTH, HEIGHT, 45.0f, glm::vec3(0,0,4));
    glfwSetCursorPos(context.getWindow(), WIDTH/2, HEIGHT/2);


    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLuint texbuffer;
    glGenTextures(1, &texbuffer);
    glBindTexture(GL_TEXTURE_2D, texbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texbuffer, 0);
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);  
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


	do{
		cam.loop();

		float x = 1.0f * sin(glfwGetTime());
		float z = 1.0f * cos(glfwGetTime());

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

		e.vec3dic["light.pos"]=glm::vec3(x,0.0f,z);
		e.mat4dic["View"]=cam.view;
		e.mat4dic["Projection"]=cam.projection;
		e.vec3dic["viewPos"]=cam.getPos();
		e.render();

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        o.vec3dic["light.pos"]=glm::vec3(x,0.0f,z);
		o.mat4dic["View"]=cam.view;
		o.mat4dic["Projection"]=cam.projection;
		o.vec3dic["viewPos"]=cam.getPos();
		o.render();

        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        t.texdic["texture"]=texbuffer;
        t.render();
		
		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}