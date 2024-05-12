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
	struct SRecklessAppSpecification
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

	class CWinApplication
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
		CWinApplication(const wchar_t wndClassName[], const wchar_t windowName[], std::vector<std::string> args);
		~CWinApplication();

		//! \brief Starts the Application main loop
		void             Run();
		//! \brief Shuts down the application, ends the main loop
		void             Close();
		
		float            GetTimeStamp();
		const HINSTANCE* GetInstance() const;


		//! \brief Adds an EditorLayer to the current Application
		void AddEditorLayer(const std::shared_ptr<IEditorLayer> layer) 
		{
			m_editorLayers.emplace_back(layer);
			layer->OnEditorLayerAttach();
		}

		//! \brief Tries to get a registered layer from the Editor
		//! \returns A shared pointer to the layer
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

		//! \brief Gets the Renderer of the Engine
		Renderer* GetRenderer() { return &m_renderer; }

	private:
		Renderer                  m_renderer;
		bool                      m_running;
		SRecklessAppSpecification m_specification;
		
		// Timestamps
		float m_timeStamp = 0.f;
		float m_frameStamp = 0.f;
		float m_lastFrameStamp = 0.f;

		std::vector <std::shared_ptr<IEditorLayer>> m_editorLayers;

		// Windows specific Message Handling
		static LRESULT CALLBACK AppProcedureSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT                 HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int                     HandleMessages();

		// Window Functions
		// TODO: this will be useful in the future
		void Initialize();
		void UpdateEditorLayers();
		void Shutdown();
		bool Update();
		void UpdateWindowSize();


		//////////////////////////////////////////////////////////////
		// Temp Testing Setup
		//Shader fallbackShader = Shader(L"D:\\projects\\CautionEngine\\src\\CautionEngine\\Shaders\\Fallback.hlsl");

	};
}