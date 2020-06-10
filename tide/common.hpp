#ifndef TIDE_COMMON_HPP
#define TIDE_COMMON_HPP

#include <shader.hpp>
#include <element.hpp>
#include <camera.hpp>
#include <context.hpp>

#define PI 3.1415926f

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

}

#endif