#pragma once

#include "ObjectID.h"

namespace CautionEngine::Core 
{
	class BaseObject
	{
	public:
		BaseObject() = default;

		const ObjectID& GetID() const { return m_id; }

	protected:
		ObjectID m_id;
	};
}