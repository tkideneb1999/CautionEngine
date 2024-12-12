#pragma once

#include "ObjectID.h"

namespace CautionEngine::Core
{
	class CAUTION_CLASS CObject
	{
	public:
		CObject();

		const SObjectID& GetID() const { return m_id; }

	protected:
		SObjectID m_id;
	};
}