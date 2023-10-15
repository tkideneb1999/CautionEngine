#include "Entity.h"

namespace CautionEngine::Core
{
	Entity::Entity()
		: m_name("EMPTY_ENTITY")
	{
	}

	Entity::Entity(const char* name)
		: m_name(name)
	{
	}
	void Entity::RemoveComponent(Component* component)
	{
	}
	Component* Entity::GetComponentByID(const ObjectID* id)
	{
		return nullptr;
	}
}