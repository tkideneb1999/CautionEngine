#pragma once

namespace CautionEngine::Engine
{
	class CAUTION_ENGINE_CLASS CEngine
	{
	public:
		static void Initialize();
		static void PrepareShutdown();
		static void Shutdown();
		static void Update();
	private:
		CEngine() = delete;
		~CEngine() = delete;
	};
}