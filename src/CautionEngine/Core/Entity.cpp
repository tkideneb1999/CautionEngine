#include "stdafx.h"
#include "Entity.h"

// Components
#include <Components/DefaultComponents/TransformComponent.h>

#include <memory>

namespace CautionEngine::Core
{
	CEntity::CEntity()
		: m_name("EMPTY_ENTITY")
	{
		// Default Component
		AddComponent(std::make_shared<Components::CTransformComponent>());
	}

	CEntity::CEntity(const char* name)
		: m_name(name)
	{
	}

	bool CEntity::RemoveComponent(CEntityComponent* pComponentToRemove)
	{
		for (auto& pComponent : m_components)
		{
			if (pComponent.get() == pComponentToRemove)
			{
				std::vector<std::shared_ptr<CEntityComponent>>::iterator compIter = std::find(m_components.begin(), m_components.end(), pComponent);
				if (compIter == m_components.end())
					return false;

				// Removes the element
				m_components.erase(compIter);
				return true;
			}
		}
		return false;
	}

	void CEntity::Destroy()
	{
		for (const auto& component : m_components)
		{
			component->Destroy();
		}
	}

	CEntityComponent* CEntity::GetComponentByID(const CObjectID* id)
	{
		return nullptr;
	}
}