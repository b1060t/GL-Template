#ifndef TIDE_CAMERA_HPP
#define TIDE_CAMERA_HPP

#include <external.hpp>

namespace tide
{
	class Camera
	{
	private:

		double _width;
		double _height;
		GLFWwindow* _window;

		GLfloat _fov;
		glm::vec3 _pos;
        glm::quat _ori;

		GLboolean _firstRun;
		GLfloat _mouse_speed;
		GLfloat _keyboard_speed;

		GLfloat _pitch;
		GLfloat _yaw;

		double _prex;
		double _prey;

		double _last_time;
	
		void mouseEvent()
		{
			double xpos, ypos;
			glfwGetCursorPos(_window, &xpos, &ypos);
			_pitch += _mouse_speed * float(_prey - ypos);
			_yaw += _mouse_speed * float(_prex - xpos);
			_prey = ypos;
			_prex = xpos;
		}
		void keyboardEvent(float delta)
		{
			glm::quat f = _ori * glm::quat(0, 0, 0, -1) * glm::conjugate(_ori);
			glm::vec3 forward = glm::vec3(f.x, f.y, f.z);
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
			if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS){
				_pos += forward * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS){
				_pos -= forward * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS){
				_pos += right * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS){
				_pos -= right * delta * _keyboard_speed;
			}
		}
		void updateVector()
		{
			glm::quat qPitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
			glm::quat qYaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));
			_ori = glm::normalize(qYaw * qPitch);
		}

	public:

		glm::mat4 view;
		glm::mat4 projection;

		Camera(GLFWwindow* window, double width, double height, GLfloat fov=45.0f, glm::vec3 pos=glm::vec3(0,0,1), GLfloat mouseSpeed=0.01f, GLfloat keyboardSpeed=1.0f, GLfloat yaw_rad=0.0f, GLfloat pitch_rad=0.0f)
			:_fov(fov),
			_pos(pos),
			_window(window),
			_width(width),
			_height(height),
			_yaw(yaw_rad),
			_pitch(pitch_rad),
			_mouse_speed(mouseSpeed),
			_keyboard_speed(keyboardSpeed)
		{
			_firstRun = true;
			projection = glm::perspective(glm::radians(_fov), float(_width/_height), 0.1f, 100.0f);
		}
		~Camera()
		{
		
		}
		void loop()
		{
			if(_firstRun)
			{
				_firstRun = false;
				_last_time = glfwGetTime();
			    glfwGetCursorPos(_window, &_prex, &_prey);
			}
			double cur_time = glfwGetTime();
			float delta = float(cur_time - _last_time);
			mouseEvent();
			updateVector();
			keyboardEvent(delta);

			glm::mat4 translate = glm::translate(glm::mat4(1.0f), -_pos);
			view = glm::mat4_cast(glm::conjugate(_ori)) * translate;

			_last_time = cur_time;
		}
		glm::vec3 getPos()
		{
			return _pos;
		}
	};
}

#endif