#pragma once

#include <string>
#include <vector>

struct CObjectID;

namespace CautionEngine
{
	namespace Core
	{
		class CEntity;
		class CScene;

		class CAUTION_API CEntityManager
		{
		public:
			CEntityManager() = default;

			void AddEntity(CEntity* entity);

			CEntity* GetEntity(CObjectID entityId);
			CEntity* GetEntity(std::string_view entityName);

			CEntity* CreateEntity(const char* entityName, const CScene* pScene);

		private:
			std::vector<CEntity*> m_entities;
		};
	}
}