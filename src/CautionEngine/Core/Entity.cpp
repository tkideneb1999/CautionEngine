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
		//AddComponent(std::make_shared<Components::CTransformComponent>());
	}

	CEntity::CEntity(const char* name)
		: m_name(name)
		, m_pParent(nullptr)
		, m_children({})
	{
		CObject();
		// Default Component
		AddComponent(std::make_shared<Components::CTransformComponent>());
	}

	CEntity::~CEntity()
	{
		// TODO: proper logging
		printf("Destroying %s \n", m_name.c_str());
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

	void CEntity::AddChild(CEntity* pEntity)
	{
		// TODO: check if object is in entity;
		if (m_pParent == pEntity->GetParent())
		{
			// TODO: log
			return;
		}

		// Sets the parent object
		pEntity->SetParent(this);
		m_children.emplace_back(pEntity);
	}

	void CEntity::SetParent(CEntity* pParent)
	{
		if (m_pParent)
		{
			// TODO: notify previous parent of child
		}
		m_pParent = pParent;
	}

	CEntityComponent* CEntity::GetComponentByID(const SObjectID* id)
	{
		return nullptr;
	}
}