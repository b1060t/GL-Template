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
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(context.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	do
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

		e.render(GL_POINTS);
		c.render(GL_POINTS);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		context.loop();
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}