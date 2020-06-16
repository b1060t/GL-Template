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

int main()
{
	Context context("Cubemap");
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);
}