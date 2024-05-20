#pragma once

#include <vector>
#include <string>
namespace CautionEngine::Core
{
	// NOTE: this is probably where we would need to investigate how to integrate the USD API
	class CEntity;
	class CScene 
	{
	public:
		CScene();
		CScene(const char* name);
		
		void Render();
		void Destroy();

		__forceinline std::vector<CEntity*> GetEntities() const
		{
			return m_entities;
		}

		void AddEntity(CEntity* pEntity);

	private:
		std::string m_sceneName;
		std::vector<CEntity*> m_entities;
	};
}