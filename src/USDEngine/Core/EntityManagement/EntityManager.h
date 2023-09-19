#pragma once

#include "BaseObject.h"

#include <string>

namespace USDEngine::Core
{
	/*class BaseObject;*/
	class Entity : public BaseObject
	{
	public:
		Entity() = default;

		void AddComponent();

	private:
		std::string m_name;
	};
}