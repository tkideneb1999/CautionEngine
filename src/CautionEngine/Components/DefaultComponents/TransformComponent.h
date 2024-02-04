#pragma once

#include <Core/ObjectID.h>
#include <Components/Component.h>

#include <CautionDefinitions.h>

namespace CautionEngine::Components
{
	class CAUTION_API TransformComponent : public Component
	{
	public: 
		// TODO: benedikt -> your math library here?
		TransformComponent();
		~TransformComponent();
	};
}