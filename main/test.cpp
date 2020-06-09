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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

using namespace glm;
using namespace tide;

extern const char _binary_shaders_shade_vert_start, _binary_shaders_shade_vert_end;
extern const char _binary_shaders_shade_frag_start, _binary_shaders_shade_frag_end;
extern const char _binary_shaders_outline_frag_start, _binary_shaders_outline_frag_end;
extern const char _binary_misc_diffuse_jpg_start, _binary_misc_diffuse_jpg_end;
extern const char _binary_misc_specular_jpg_start, _binary_misc_specular_jpg_end;
extern const char _binary_misc_normal_png_start, _binary_misc_normal_png_end;
extern const char _binary_misc_a_obj_start, _binary_misc_a_obj_end;

#define WIDTH 1024
#define HEIGHT 768

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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Test", NULL, NULL);
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
	Attr a1, a2, a3;
	a1.size=3; a1.norm=GL_FALSE; a1.type=GL_FLOAT; a1.stride=8; a1.offset=0;
	a2.size=2; a2.norm=GL_FALSE; a2.type=GL_FLOAT; a2.stride=8; a2.offset=3;
	a3.size=3; a3.norm=GL_FALSE; a3.type=GL_FLOAT; a3.stride=8; a3.offset=5;
	v.push_back(a1); v.push_back(a2); v.push_back(a3);

	mango::Memory diffuse_mem((unsigned char*)(&_binary_misc_diffuse_jpg_start), (&_binary_misc_diffuse_jpg_end-&_binary_misc_diffuse_jpg_start)*sizeof(unsigned char));
	mango::Bitmap diffuse_tex(diffuse_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory specular_mem((unsigned char*)(&_binary_misc_specular_jpg_start), (&_binary_misc_specular_jpg_end-&_binary_misc_specular_jpg_start)*sizeof(unsigned char));
	mango::Bitmap specular_tex(specular_mem, ".jpg", mango::FORMAT_B8G8R8);
	mango::Memory normal_mem((unsigned char*)(&_binary_misc_normal_png_start), (&_binary_misc_normal_png_end-&_binary_misc_normal_png_start)*sizeof(unsigned char));
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
	    std::string(&_binary_shaders_shade_vert_start, &_binary_shaders_shade_vert_end - &_binary_shaders_shade_vert_start),
	    std::string(&_binary_shaders_shade_frag_start, &_binary_shaders_shade_frag_end - &_binary_shaders_shade_frag_start)
	);

    Shader outline_shader(
        std::string(&_binary_shaders_shade_vert_start, &_binary_shaders_shade_vert_end - &_binary_shaders_shade_vert_start),
	    std::string(&_binary_shaders_outline_frag_start, &_binary_shaders_outline_frag_end - &_binary_shaders_outline_frag_start)
    );


	Assimp::Importer importer;
	//const aiScene* sceneObjPtr = importer.ReadFile("a.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene* sceneObjPtr = importer.ReadFileFromMemory((void*)(&_binary_misc_a_obj_start), (&_binary_misc_a_obj_end-&_binary_misc_a_obj_start)*sizeof(char),aiProcess_ValidateDataStructure);
	Element e(sceneObjPtr, v);
	e.attachShader(&shade_shader);
	e.addTexture("material.diffuse", diffuse);
	e.addTexture("material.specular", specular);
	e.addTexture("material.normal", normal);
	e.addFloatUniform("material.shiness", 32.0f);
	e.addVec3Uniform("light.pos", glm::vec3(1.0f,0.0f,0.0f));
	e.addVec3Uniform("light.diffuse", glm::vec3(0.7f));
	e.addVec3Uniform("light.ambient", glm::vec3(0.2f));
	e.addVec3Uniform("light.specular", glm::vec3(1.0f));
	e.addMat4Uniform("View", glm::mat4(1.0f));
	e.addMat4Uniform("Projection", glm::mat4(1.0f));
	e.addVec3Uniform("viewPos", glm::vec3(0.0f));

    Element o(sceneObjPtr, v);
	o.attachShader(&outline_shader);
	o.addTexture("material.diffuse", diffuse);
	o.addTexture("material.specular", specular);
	o.addTexture("material.normal", normal);
	o.addFloatUniform("material.shiness", 32.0f);
	o.addVec3Uniform("light.pos", glm::vec3(1.0f,0.0f,0.0f));
	o.addVec3Uniform("light.diffuse", glm::vec3(0.7f));
	o.addVec3Uniform("light.ambient", glm::vec3(0.2f));
	o.addVec3Uniform("light.specular", glm::vec3(1.0f));
	o.addMat4Uniform("View", glm::mat4(1.0f));
	o.addMat4Uniform("Projection", glm::mat4(1.0f));
	o.addVec3Uniform("viewPos", glm::vec3(0.0f));

    e.internal_model = true;
    e.setPosition(glm::vec3(-0.1f,-0.2f,-0.3f));
    e.setScale(glm::vec3(0.8f));
    e.setRotation(glm::vec3(0.0f,0.0f,0.0f));

    o.internal_model = true;
    o.setPosition(glm::vec3(-0.1f,-0.2f,-0.3f));
    o.setScale(glm::vec3(0.82f));
    o.setRotation(glm::vec3(0.0f,0.0f,0.0f));

	Camera cam(window, WIDTH, HEIGHT, 45.0f, glm::vec3(0,0,4));
    glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		cam.loop();

		float x = 1.0f * sin(glfwGetTime());
		float z = 1.0f * cos(glfwGetTime());

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
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}