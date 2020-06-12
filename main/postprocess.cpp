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

extern "C" {
	extern const char _binary_shaders_tex_vert_start, _binary_shaders_tex_vert_end;
	extern const char _binary_shaders_tex_frag_start, _binary_shaders_tex_frag_end;
    extern const char _binary_shaders_mean_frag_start, _binary_shaders_mean_frag_end;
    extern const char _binary_shaders_corr_frag_start, _binary_shaders_corr_frag_end;
    extern const char _binary_shaders_alpha_frag_start, _binary_shaders_alpha_frag_end;
    extern const char _binary_shaders_beta_frag_start, _binary_shaders_beta_frag_end;
    extern const char _binary_shaders_guided_frag_start, _binary_shaders_guided_frag_end;
	extern const char _binary_misc_postprocess_jpg_start, _binary_misc_postprocess_jpg_end;
    extern const char _binary_misc_guided_png_start, _binary_misc_guided_png_end;
}

int main()
{
    mango::Memory img_mem((unsigned char*)(&_binary_misc_guided_png_start), (&_binary_misc_guided_png_end-&_binary_misc_guided_png_start)*sizeof(unsigned char));
    mango::Bitmap img_tex(img_mem, ".png", mango::FORMAT_B8G8R8);
    Context context("Test", img_tex.width, img_tex.height);
    glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

    GLuint img;
    glGenTextures(1, &img);
	glBindTexture(GL_TEXTURE_2D, img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_tex.width, img_tex.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img_tex.image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    Shader tex_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_tex_frag_start, &_binary_shaders_tex_frag_end - &_binary_shaders_tex_frag_start)
	);

    Shader mean_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_mean_frag_start, &_binary_shaders_mean_frag_end - &_binary_shaders_mean_frag_start)
	);

    Shader corr_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_corr_frag_start, &_binary_shaders_corr_frag_end - &_binary_shaders_corr_frag_start)
	);

    Shader alpha_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_alpha_frag_start, &_binary_shaders_alpha_frag_end - &_binary_shaders_alpha_frag_start)
	);

    Shader beta_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_beta_frag_start, &_binary_shaders_beta_frag_end - &_binary_shaders_beta_frag_start)
	);

    Shader guided_shader(
	    std::string(&_binary_shaders_tex_vert_start, &_binary_shaders_tex_vert_end - &_binary_shaders_tex_vert_start),
	    std::string(&_binary_shaders_guided_frag_start, &_binary_shaders_guided_frag_end - &_binary_shaders_guided_frag_start)
	);

    tide::Element mean(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    mean.attachShader(&mean_shader);
    mean.addTexture("tex", img);
    mean.addFloatUniform("width", img_tex.width);
    mean.addFloatUniform("height", img_tex.height);
    mean.internal_model = false;
    glClear(GL_COLOR_BUFFER_BIT);
    Frame fmean(img_tex.width, img_tex.height);
    fmean.bind();
    mean.render();
    fmean.unbind();

    tide::Element corr(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    corr.attachShader(&corr_shader);
    corr.addTexture("tex", img);
    corr.addFloatUniform("width", img_tex.width);
    corr.addFloatUniform("height", img_tex.height);
    corr.internal_model = false;
    glClear(GL_COLOR_BUFFER_BIT);
    Frame fcorr(img_tex.width, img_tex.height);
    fcorr.bind();
    corr.render();
    fcorr.unbind();

    tide::Element alpha(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    alpha.attachShader(&alpha_shader);
    alpha.addTexture("corrt", fcorr.getTex());
    alpha.addTexture("meant", fmean.getTex());
    alpha.internal_model = false;
    glClear(GL_COLOR_BUFFER_BIT);
    Frame falpha(img_tex.width, img_tex.height);
    falpha.bind();
    alpha.render();
    falpha.unbind();

    tide::Element beta(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    beta.attachShader(&beta_shader);
    beta.addTexture("meant", fmean.getTex());
    beta.addTexture("at", falpha.getTex());
    beta.internal_model = false;
    glClear(GL_COLOR_BUFFER_BIT);
    Frame fbeta(img_tex.width, img_tex.height);
    fbeta.bind();
    beta.render();
    fbeta.unbind();

    tide::Element final(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), 16*sizeof(GLfloat), 6*sizeof(GLint), tide::TWOD_TEXTURE_ATTR);
    final.attachShader(&guided_shader);
    final.addTexture("at", falpha.getTex());
    final.addTexture("bt", fbeta.getTex());
    final.addTexture("imgt", img);
    final.internal_model = false;
    glClear(GL_COLOR_BUFFER_BIT);
    Frame ffinal(img_tex.width, img_tex.height);
    ffinal.bind();
    final.render();
    ffinal.unbind();

    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        final.render();
        context.loop();
    }
    while(context.shouldClose(GLFW_KEY_ESCAPE));

    return 0;
}