#ifndef TIDE_RESOURCE_HPP
#define TIDE_RESOURCE_HPP

#include <external.hpp>

namespace tide
{
	class Texture
	{

	};
	class Model
	{

	};
	//use singleton?
	class ResourcePool
	{
	private:
		std::map<std::string, GLuint> _texdic;
		std::map<std::string, Shader*> _shaderdic;
		ResourcePool()
		{

		}
	public:
		~ResourcePool()
		{
			for(auto t : _texdic)
			{
				glDeleteTextures(1, &t.second);
			}
			for(auto s : _shaderdic)
			{
				glDeleteProgram(s.second->handle);
			}
		}
		static ResourcePool& getInstance()
		{
			static ResourcePool _instance;
			return _instance;
		}
		ResourcePool(const ResourcePool&)=delete;
		ResourcePool& operator=(const ResourcePool&)=delete;
//logic
		GLuint loadImageAsTexture(std::string name, unsigned char* ptr, size_t size, std::string&& ext=".jpg", mango::Format format=mango::FORMAT_B8G8R8)
		{
			mango::Memory mem(ptr, size);
			mango::Bitmap img(mem, ext, format);
			GLuint handle;
			glGenTextures(1, &handle);
			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_BGR, GL_UNSIGNED_BYTE, img.image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			_texdic[name] = handle;
			return handle;
		}
		void loadShader(std::string name, std::string&& vertex, std::string&& geom, std::string&& frag)
		{
			Shader shader(vertex, geom, frag);
			_shaderdic[name] = &shader;
		}
		void loadShader(std::string name, std::string&& vertex, std::string&& frag)
		{
			Shader shader(vertex, frag);
			_shaderdic[name] = &shader;
		}
	};
}
#endif