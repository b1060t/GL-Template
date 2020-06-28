#ifndef TIDE_RESOURCE_HPP
#define TIDE_RESOURCE_HPP

#include <external.hpp>
#include <element.hpp>

namespace tide
{
	//use singleton?
	class ResourcePool
	{
	private:
		ThreadPool* pool = ThreadPool::getResourcePool();
		inline ResourcePool()
		{

		}
	public:
		std::map<std::string, Texture*> texdic;
		std::map<std::string, Shader*> shaderdic;
		std::map<std::string, Model*> modeldic;
		std::map<std::string, Light*> lightdic;
		inline ~ResourcePool()
		{
			for(auto t : texdic)
			{
				if(t.second)
				{
					delete t.second;
					t.second == nullptr;
				}
			}
			texdic.clear();
			for(auto s : shaderdic)
			{
				if(s.second)
				{
					delete s.second;
					s.second == nullptr;
				}
			}
			shaderdic.clear();
			for(auto m : modeldic)
			{
				if(m.second)
				{
					delete m.second;
					m.second == nullptr;
				}
			}
			modeldic.clear();
		}
		static ResourcePool& getResourcePool()
		{
			static ResourcePool _instance;
			return _instance;
		}
		ResourcePool(const ResourcePool&)=delete;
		ResourcePool& operator=(const ResourcePool&)=delete;
//logic
		void loadTexture(std::string name, const char* ptr, size_t size, std::string&& ext=".jpg", mango::Format format=mango::FORMAT_B8G8R8)
		{
			Texture* t = new Texture(ptr, size, ext, format);
			texdic[name] = t;
		}
		void loadShader(std::string name, std::string&& vertex, std::string&& geom, std::string&& frag)
		{
			Shader* s = new Shader(vertex, geom, frag);
			shaderdic[name] = s;
		}
		void loadShader(std::string name, std::string&& vertex, std::string&& frag)
		{
			Shader* s = new Shader(vertex, frag);
			shaderdic[name] = s;
		}
		void loadModel(std::string name, char* ptr, size_t size)
		{
			Model* m = new Model(ptr, size);
			modeldic[name] = m;
		}
	};
}
#endif