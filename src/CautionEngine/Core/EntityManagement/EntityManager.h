#pragma once

#include <string>
#include <vector>

namespace CautionEngine::Core
{
	class CEntity;
	class CObjectID;
	
	class EntityManager
	{
	public:
		EntityManager() = default;

		void AddEntity(CEntity* entity);

		CEntity* GetEntity(CObjectID entityId);
		CEntity* GetEntity(std::string_view entityName);

	private:
		std::vector<CEntity*> m_entities;
	};
}