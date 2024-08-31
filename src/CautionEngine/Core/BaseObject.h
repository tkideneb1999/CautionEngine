#pragma once

#include "ObjectID.h"

namespace CautionEngine::Core
{
	class CAUTION_API CObject
	{
	public:
		CObject() = default;

		const CObjectID& GetID() const { return m_id; }

	protected:
		CObjectID m_id;
	};
}