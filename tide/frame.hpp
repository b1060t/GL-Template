#ifndef TIDE_FRAME_HPP
#define TIDE_FRAME_HPP

#include <external.hpp>

namespace tide
{
	class Frame
	{
	private:
		GLuint _fbo;
		std::vector<GLuint> _tex;
		std::vector<GLuint> _attachment;
		GLuint _rbo;

		double _width;
		double _height;

		std::vector<Element> _elements;

		GLbitfield _clearFlag;

    public:
		Frame(double width, double height, std::vector<GLuint> attachment={GL_COLOR_ATTACHMENT0})
			:_width(width),
			_height(height),
			_attachment(attachment),
			_clearFlag(0)
		{
			glGenFramebuffers(1, &_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			//configure attachment
			for(auto a : _attachment)
			{
				GLuint t;
				glGenTextures(1, &t);
				glBindTexture(GL_TEXTURE_2D, t);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, a, GL_TEXTURE_2D, t, 0);
				_tex.push_back(t);
				//configure clear flag
				if(a >= GL_COLOR_ATTACHMENT0 && a < GL_DEPTH_ATTACHMENT) _clearFlag |= GL_COLOR_BUFFER_BIT;
				if(a == GL_DEPTH_ATTACHMENT) _clearFlag |= GL_DEPTH_BUFFER_BIT;
				if(a == GL_STENCIL_ATTACHMENT) _clearFlag |= GL_STENCIL_BUFFER_BIT;
			}
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}
		~Frame()
		{
			glDeleteTextures(_tex.size(), &_tex[0]);
		}
		void generateRBO(GLuint datatype, GLuint attachment)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			glGenRenderbuffers(1, &_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _rbo); 
			glRenderbufferStorage(GL_RENDERBUFFER, datatype, _width, _height);  
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, _rbo);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void bind()
		{
			int num = _tex.size();
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			std::vector<GLenum> color_buffers;
			for(auto a : _attachment)
			{
				if(a >= GL_COLOR_ATTACHMENT0 && a < GL_DEPTH_ATTACHMENT)
				{
					color_buffers.push_back(a);
				}
			}
			if(color_buffers.size() > 0)
			{
				glDrawBuffers(color_buffers.size(), &color_buffers[0]);
			}
			else
			{
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}
		}
		void unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		GLuint getTex(int index=0)
		{
			return _tex[index];
		}
		void clear()
		{
			glViewport(0, 0, _width, _height);
			glClear(_clearFlag);
		}
	};
}

#endif