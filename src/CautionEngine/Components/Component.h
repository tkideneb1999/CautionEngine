#pragma once

#include <Core/ObjectID.h>

class Entity;

namespace CautionEngine::Components
{
	class Component
	{
	public:
		Component() = default;

		void Register();
		void Unregister();

		void Update();
		void Destroy();

		const ObjectID& GetID() const { return m_id; }

		Entity* GetEntityOwner() const { return m_owner; }
		inline void SetOwner(Entity* owner) { m_owner = owner; }
	protected:
		friend Entity;
		ObjectID m_componentId;
		ObjectID m_id;
		Entity* m_owner;
	};
}