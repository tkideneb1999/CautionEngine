#pragma once

#include <string>
#include <vector>

struct CObjectID;

namespace CautionEngine
{
	namespace Core
	{
		class CEntity;

		class CAUTION_API CEntityManager
		{
		public:
			CEntityManager() = default;

			void AddEntity(CEntity* entity);

			CEntity* GetEntity(CObjectID entityId);
			CEntity* GetEntity(std::string_view entityName);

		private:
			std::vector<CEntity*> m_entities;
		};
	}
}