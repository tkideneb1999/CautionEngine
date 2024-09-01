#pragma once

#include <Components/Component.h>

namespace CautionEngine::Components
{
	class CAUTION_CLASS CTransformComponent : public CEntityComponent
	{
	public: 
		// TODO: benedikt -> your math library here?
		CTransformComponent();
		~CTransformComponent();

		// Inherited via CEntityComponent
		void Serialize() override;
		void Deserialize() override;
	};
}