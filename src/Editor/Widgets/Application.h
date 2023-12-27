#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <memory>

#include "IEditorLayer.h"

#include "Renderer/Renderer.h"

#include "imgui.h"
#include <d3d12.h>

using namespace CautionEngine::Rendering;

namespace Reckless
{
	struct RecklessAppSpecification
	{
		std::string AppName = "RecklessEd";
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

	public:
		Application(const wchar_t wndClassName[], const wchar_t windowName[], std::vector<std::string> args);
		~Application();

		bool Update();
		float GetTimeStamp();
		HINSTANCE GetInstance() const;

		// Layer functions
		void AddEditorLayer(const std::shared_ptr<IEditorLayer> layer) 
		{
			m_editorLayers.emplace_back(layer);
			layer->OnEditorLayerAttach();
		}

	private:
		RecklessAppSpecification m_specification;

		Renderer renderer;

		bool m_running;
		
		// Timestamps
		float m_timeStamp = 0.f;
		float m_frameStamp = 0.f;
		float m_lastFrameStamp = 0.f;

		std::vector <std::shared_ptr<IEditorLayer>> m_editorLayers;

		static LRESULT CALLBACK AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int HandleMessages();

		// Window Functions
		// TODO: this will be useful in the future
		void Init();
		void Shutdown();
		void DrawEditorLayers();
	};
}