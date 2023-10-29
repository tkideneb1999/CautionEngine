#pragma once

#include <vector>
#include <string>
namespace CautionEngine::Core
{
	class Entity;
	class Scene {
	public:
		Scene();
		Scene(const char* name);
		
		void Render();
		void Destroy();

		__forceinline std::vector<Entity*> GetEntities()
		{
			return m_entities;
		}
	private:
		std::string m_sceneName;
		std::vector<Entity*> m_entities;
	};
}