#pragma once

#include "../Core/ObjectID.h"

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

	protected:
		friend Entity;

	private:
		ObjectID m_id;
	};
}