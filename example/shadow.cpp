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

int main()
{
	Context context("Shadow");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	Camera cam(context.getWindow(), WIDTH, HEIGHT, 45.0f, glm::vec3(0,0,0));
    glfwSetCursorPos(context.getWindow(), WIDTH/2, HEIGHT/2);

	do
    {
        cam.loop();
        
        context.loop();
    }
    while(context.shouldClose(GLFW_KEY_ESCAPE));

	return 0;
}