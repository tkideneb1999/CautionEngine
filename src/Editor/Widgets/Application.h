#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <memory>

#include "EditorLayer.h"

#include "imgui.h"
#include <d3d12.h>

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
		HINSTANCE hInstance;

	private:
		WNDCLASS windowClass;
		std::vector<std::string> args;

		MSG currentMsg;
		BOOL currentMsgResult;

	public:
		Application(const wchar_t wndClassName[], const wchar_t windowName[], std::vector<std::string> args);

		bool Update();
		float GetTimeStamp();

		// Layer functions
		void AddEditorLayer(const std::shared_ptr<EditorLayer> layer) 
		{
			m_editorLayers.emplace_back(layer);
			layer->OnEditorLayerAttach();
		}

	private:
		RecklessAppSpecification m_specification;

		bool m_running;
		
		// Timestamps
		float m_timeStamp = 0.f;
		float m_frameStamp = 0.f;
		float m_lastFrameStamp = 0.f;

		std::vector <std::shared_ptr<EditorLayer>> m_editorLayers;

		static LRESULT CALLBACK AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int HandleMessages();

		// Window Functions
		// TODO: this will be useful in the future
		void Init();
		void Shutdown();
	};
}