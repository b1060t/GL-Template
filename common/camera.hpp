#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <common.hpp>

namespace tide
{
	#define MAX_PITCH_ANGLE 3.14f/4.0f
	#define MIN_PITCH_ANGLE -3.14f/4.0f
	#define MAX_YAW_ANGLE 3.14f
	#define MIN_YAW_ANGLE 0.0f
	
	class Camera
	{
	private:
	
		double _width;
		double _height;
		GLFWwindow* _window;
	
		GLfloat _fov;
		glm::vec3 _pos;
	
		GLboolean _firstRun;
		GLfloat _mouse_speed;
		GLfloat _keyboard_speed;
	
		GLfloat _pitch;
		GLfloat _yaw;
	
		glm::vec3 _forward;
		glm::vec3 _right;
		glm::vec3 _up;
	
		double _last_time;
	
		void mouseEvent()
		{
			double xpos, ypos;
			glfwGetCursorPos(_window, &xpos, &ypos);
			glfwSetCursorPos(_window, _width/2, _height/2);
			_pitch += _mouse_speed * float(_height/2 - ypos);
			_yaw += _mouse_speed * float(_width/2 - xpos);
			if (_pitch > MAX_PITCH_ANGLE)
	    	    _pitch = MAX_PITCH_ANGLE;
	    	if (_pitch < MIN_PITCH_ANGLE)
	    	    _pitch = MIN_PITCH_ANGLE;
	    	if (_yaw < MIN_YAW_ANGLE)
	    	    _yaw += 3.14f;
		}
		void keyboardEvent(float delta)
		{
			if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS){
				_pos += _forward * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS){
				_pos -= _forward * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS){
				_pos += _right * delta * _keyboard_speed;
			}
			if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS){
				_pos -= _right * delta * _keyboard_speed;
			}
		}
		void updateVector()
		{
			_forward.x = sin(_yaw) * cos(_pitch);
			_forward.y = sin(_pitch);
			_forward.z = cos(_yaw) * cos(_pitch);
			_forward = glm::normalize(_forward);
	
			_right.x = sin(_yaw - 3.14f/2.0f);
			_right.y = 0;
			_right.z = cos(_yaw - 3.14f/2.0f);
			_right = glm::normalize(_right);
	
			_up = glm::cross(_right, _forward);
		}
	public:
	
		glm::mat4 view;
		glm::mat4 projection;
	
		Camera(GLFWwindow* window, double width, double height, GLfloat fov=45.0f, glm::vec3 pos=glm::vec3(0,0,1), GLfloat mouseSpeed=0.01f, GLfloat keyboardSpeed=1.0f)
			:_fov(fov),
			_pos(pos),
			_window(window),
			_width(width),
			_height(height),
			_yaw(3.14f),
			_pitch(0),
			_mouse_speed(mouseSpeed),
			_keyboard_speed(keyboardSpeed)
		{
			_firstRun = true;
			
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
			}
			double cur_time = glfwGetTime();
			float delta = float(cur_time - _last_time);
			mouseEvent();
			updateVector();
			keyboardEvent(delta);
			projection = glm::perspective(glm::radians(_fov), float(_width/_height), 0.1f, 100.0f);
			view = glm::lookAt(
				_pos,
				_pos+_forward,
				_up
			);
			_last_time = cur_time;
		}
		
	};
}

#endif