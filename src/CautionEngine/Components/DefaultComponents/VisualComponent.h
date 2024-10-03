#pragma once

#include <Components/Component.h>

namespace CautionEngine::Components
{
	class CVisualComponent : public CEntityComponent
	{
	public:
		CVisualComponent();
		~CVisualComponent();

		// Inherited via CEntityComponent
		void Serialize() override;
		void Deserialize() override;
	};
}