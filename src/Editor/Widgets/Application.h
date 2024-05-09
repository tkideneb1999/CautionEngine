#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>

#include "IEditorLayer.h"

#include "Renderer/Renderer.h"

#include "imgui.h"
#include <d3d12.h>

///////////////////////////////
// Temp Testing Setup
#include "Renderer/Shader.h"

///////////////////////////////

using namespace CautionEngine::Rendering;

namespace Reckless
{
	struct RecklessAppSpecification
	{
		std::string AppName = "RecklessEd";
	};

	enum WINDOW_STATE
	{
		WINDOW_STATE_DEFAULT    = 0,
		WINDOW_STATE_RESIZING   = 1,
		WINDOW_STATE_MINIMIZED  = 2,
		WINDOW_STATE_MAXIMIZED  = 3,
		WINDOW_STATE_FULLSCREEN = 4,
	};

	class Application
	{
	public:
		HWND hWnd;

	private:
		WNDCLASS windowClass;
		HINSTANCE hInstance;

		std::vector<std::string> args;

		MSG currentMsg;
		BOOL currentMsgResult;

		unsigned int m_width = 0;
		unsigned int m_height = 0;
		
		WINDOW_STATE applicationState = WINDOW_STATE_DEFAULT;

	public:
		Application(const wchar_t wndClassName[], const wchar_t windowName[], std::vector<std::string> args);
		~Application();

		bool Update();
		float GetTimeStamp();
		const HINSTANCE* GetInstance() const;

		// Layer functions
		void AddEditorLayer(const std::shared_ptr<IEditorLayer> layer) 
		{
			m_editorLayers.emplace_back(layer);
			layer->OnEditorLayerAttach();
		}

		template<typename T>
		std::shared_ptr<T> GetEditorLayer()
		{
			for (const auto& pLayer : m_editorLayers)
			{
				if (std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(pLayer))
				{
					if (ptr)
						return ptr;
				}
			}
			return nullptr;
		}

	private:
		RecklessAppSpecification m_specification;

		Renderer m_renderer;

		bool m_running;
		
		// Timestamps
		float m_timeStamp = 0.f;
		float m_frameStamp = 0.f;
		float m_lastFrameStamp = 0.f;

		std::vector <std::shared_ptr<IEditorLayer>> m_editorLayers;

		// Windows specific Message Handling
		static LRESULT CALLBACK AppProcedureSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int HandleMessages();

		// Window Functions
		// TODO: this will be useful in the future
		void Init();
		void Shutdown();
		void DrawEditorLayers();

		void UpdateWindowSize();
		void HandleLMB();

		//////////////////////////////////////////////////////////////
		// Temp Testing Setup
		//Shader fallbackShader = Shader(L"D:\\projects\\CautionEngine\\src\\CautionEngine\\Shaders\\Fallback.hlsl");

	};
}