#pragma once

#include <string>
#include <vector>

namespace CautionEngine::Core
{
	class Entity;
	/*class BaseObject;*/
	class EntityManager
	{
	public:
		EntityManager() = default;

		void AddEntity(Entity* entity);

	private:
		std::vector<Entity*> m_entities;
	};
}