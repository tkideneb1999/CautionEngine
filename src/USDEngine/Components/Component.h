#pragma once

namespace USDEngine::Components
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