#ifndef TIDE_CONTEXT_HPP
#define TIDE_CONTEXT_HPP

#include <external.hpp>

namespace tide
{
	class Context
	{
	private:
		double _width;
		double _height;
		GLFWwindow* _window;
	public:
		Context(const std::string &name, double width=1024, double height=768, int samples=4, int major=3, int minor=3, int profile=GLFW_OPENGL_CORE_PROFILE)
			:_width(width),
			_height(height)
		{
			if(!glfwInit())
			{
				fprintf(stderr, "Failed to initialize GLFW\n");
			}
			glfwWindowHint(GLFW_SAMPLES, samples);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
			_window = glfwCreateWindow(_width, _height, name.c_str(), NULL, NULL);
			if(_window == NULL)
			{
				fprintf(stderr, "Failed to open GLFW window.\n");
				glfwTerminate();
			}
			glfwMakeContextCurrent(_window);
			glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
			if (glewInit() != GLEW_OK)
			{
			    fprintf(stderr, "Failed to initialize GLEW\n");
			    glfwTerminate();
			}
		}
		~Context()
		{
			glfwTerminate();
		}

		GLFWwindow* getWindow()
		{
			return _window;
		}
		void loop()
		{
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}
		bool shouldClose(int key)
		{
			return (glfwGetKey(_window, key) != GLFW_PRESS && glfwWindowShouldClose(_window) == 0);
		}
		void terminate()
		{
			glfwTerminate();
		}
	};
}

#endif