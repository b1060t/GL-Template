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
        GLuint _rbo;

        double _width;
        double _height;

        std::vector<Element> _elements;
        
    public:
        Frame(double width, double height, int attachmentNum=1)
            :_width(width),
            _height(height)
        {
            glGenFramebuffers(1, &_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            for(int i = 0; i < attachmentNum; i++)
            {
                GLuint t;
                glGenTextures(1, &t);
                glBindTexture(GL_TEXTURE_2D, t);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, t, 0);
                _tex.push_back(t);
            }
            
            glGenRenderbuffers(1, &_rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, _rbo); 
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);  
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        ~Frame()
        {
            glDeleteTextures(_tex.size(), &_tex[0]);
        }
        void bind()
        {
            int num = _tex.size();
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            std::vector<GLenum> buffers;
            for(int i = 0; i < num; i++)
            {
                buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            }
            glDrawBuffers(num, &buffers[0]);
        }
        void unbind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        GLuint getTex(int index=0)
        {
            return _tex[index];
        }
    };
}

#endif