#ifndef TIDE_COMMON_HPP
#define TIDE_COMMON_HPP

#include <shader.hpp>
#include <element.hpp>
#include <camera.hpp>
#include <context.hpp>

#define PI 3.1415926f

//#ifdef __APPLE__
//#include <mach-o/getsect.h>
//#define EXTLD(NAME) \
//  extern const char _section$__DATA__ ## NAME 
//#define LDVAR(NAME) (&_section$__DATA__ ## NAME )
//#define LDLEN(NAME) (getsectiondata("__DATA", "__" #NAME)->size)
//#else /* gnu/linux ld */
#define EXTLD(NAME) \
  extern const char _binary_ ## NAME ## _start; \
  extern const char _binary_ ## NAME ## _end
#define LDVAR(NAME) \
  (&_binary_ ## NAME ## _start)
#define LDLEN(NAME) \
  ((&(_binary_ ## NAME ## _end) - &(_binary_ ## NAME ## _start)) * sizeof(char))
//#endif

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
}

#endif