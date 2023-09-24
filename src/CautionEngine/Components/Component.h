#pragma once

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
	};
}