#ifndef TIDE_COMMON_HPP
#define TIDE_COMMON_HPP

#include <shader.hpp>
#include <element.hpp>
#include <camera.hpp>
#include <context.hpp>

#define PI 3.1415926f

#ifdef __APPLE__
#define EXTLD(NAME) \
  extern "C" const char NAME ; \
  extern "C" const int NAME ## _length
#define LDVAR(NAME) (& NAME )
#define LDLEN(NAME) (NAME ## _length)
#else /* gnu/linux ld */
#define EXTLD(NAME) \
  extern "C" const char _binary_ ## NAME ## _start; \
  extern "C" const char _binary_ ## NAME ## _end
#define LDVAR(NAME) \
  (&_binary_ ## NAME ## _start)
#define LDLEN(NAME) \
  ((&(_binary_ ## NAME ## _end) - &(_binary_ ## NAME ## _start)) * sizeof(char))
#endif

namespace tide
{

	static const std::vector<Attr> THREED_OBJECT_ATTR = {
		{3, GL_FLOAT, GL_FALSE, 8, 0}, //position x,y,z
		{2, GL_FLOAT, GL_FALSE, 8, 3}, //uv coordinates u,v
		{3, GL_FLOAT, GL_FALSE, 8, 5}  //normal x,y,z
	};
	static const std::vector<Attr> TWOD_TEXTURE_ATTR = {
		{2, GL_FLOAT, GL_FALSE, 4, 0}, //position x,y
		{2, GL_FLOAT, GL_FALSE, 4, 2}  //uv coordinates u,v
	};
	static const std::vector<Attr> TWOD_POINT_ATTR = {
		{2, GL_FLOAT, GL_FALSE, 2, 0}, //position x,y
	};
	static const std::vector<Attr> TWOD_COLORED_POINT_ATTR = {
		{2, GL_FLOAT, GL_FALSE, 5, 0}, //position x,y
		{3, GL_FLOAT, GL_FALSE, 5, 2}, //color r,g,b
	};
	static const std::vector<Attr> SKYBOX_ATTR = {
		{3, GL_FLOAT, GL_FALSE, 3, 0}, //position x,y,z
	};

	static const std::vector<GLuint> FRAME_ATTACHMENTS_2COLOR = 
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};

	static const std::vector<GLuint> FRAME_ATTACHMENTS_1DEPTH = 
	{
		GL_DEPTH_ATTACHMENT
	};

	static const GLfloat ImageVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	static const GLint ImageIndices[] {
		0, 1, 2,
		0, 2, 3
	};
	static const GLfloat CubeVertices[] = {
		// positions          // texCoords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};
	static const GLint CubeIndices[] {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10,  4,  8,
		11,  2, 12, 12, 13, 11,
		10, 14,  5,  5,  4, 10,
		 3,  2, 11, 11, 15,  3
	};
	static const GLfloat SkyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f
	};
	static const GLint SkyboxIndices[] {
		0, 1, 2, 2, 3, 0,
		4, 1, 0, 0, 5, 4,
		2, 6, 7, 7, 3, 2,
		4, 5, 7, 7, 6, 4,
		0, 3, 7, 7, 5, 0,
		1, 4, 2, 2, 4, 6
	};
}

#endif