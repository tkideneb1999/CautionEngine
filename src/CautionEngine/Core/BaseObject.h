#pragma once

#include "ObjectID.h"

namespace CautionEngine::Core
{
	class CObject
	{
	public:
		CObject() = default;

		const CObjectID& GetID() const { return m_id; }

		void SetName(std::string_view name) { m_name = name; }
		std::string_view GetName() const { return m_name; }

	protected:
		CObjectID     m_id;
		std::string   m_name;
	};
}