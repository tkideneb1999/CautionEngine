#pragma once

#include <Components/Component.h>

namespace CautionEngine::Components
{
	class CAUTION_API CVisualComponent : public CEntityComponent
	{
	public:
		CVisualComponent();
		~CVisualComponent();

		// Inherited via CEntityComponent
		void Serialize() override;
		void Deserialize() override;
		void Update() override;
		void Destroy() override;
	};
}