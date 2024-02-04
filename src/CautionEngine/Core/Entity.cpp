#include "stdafx.h"
#include "Entity.h"

// Components
#include <Components/DefaultComponents/TransformComponent.h>

#include <memory>

namespace CautionEngine::Core
{
	Entity::Entity()
		: m_name("EMPTY_ENTITY")
	{
		// Default Component
		AddComponent(std::make_shared<Components::TransformComponent>());
	}

	Entity::Entity(const char* name)
		: m_name(name)
	{
	}
	void Entity::RemoveComponent(Component* component)
	{
	}
	void Entity::Destroy()
	{
		for (const auto& component : m_components)
		{
			component->Destroy();
		}
	}

	Component* Entity::GetComponentByID(const ObjectID* id)
	{
		return nullptr;
	}
}