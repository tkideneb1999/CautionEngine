#include "Application.h"

#include <stdexcept>

#include "imgui.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

namespace Reckless {
	Application::Application(const wchar_t wndClassName[], const wchar_t wndName[], std::vector<std::string> lArgs)
		: args(lArgs)
	{
		hInstance = GetModuleHandle(nullptr);

		windowClass = { 0 };
		windowClass.lpfnWndProc = AppProcedure;
		windowClass.hInstance = hInstance;
		windowClass.lpszClassName = wndClassName;

		RegisterClass(&windowClass);

		hWnd = CreateWindowEx(
			0,
			wndClassName,
			wndName,
			WS_OVERLAPPEDWINDOW,

			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		if (hWnd == nullptr) {
			throw std::runtime_error("Error Initializing Window");
		}

		ShowWindow(hWnd, SW_SHOW);

		Init();
	}

	Application::~Application()
	{
		Shutdown();
	}

	bool Application::Update()
	{
		// Handle Messages
		if (HandleMessages() != 1) 
		{
			// Do Shutdown here
			return false;
		}
		// TODO: More Update Code

		for (auto editorLayers : m_editorLayers)
		{
			// TODO: we need to pass in the parameter of the timestamp here...
			editorLayers->Update();
		}

		return true;
	}

	float Application::GetTimeStamp()
	{
		// TODO: implement, we might need to look for dx12 related timestamps/frequency here...
		return 0.0f;
	}

	LRESULT CALLBACK Application::AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		switch (msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	int Application::HandleMessages()
	{
		while (PeekMessage(&currentMsg, nullptr, 0, 0, PM_REMOVE)) 
		{
			if (currentMsg.message == WM_QUIT) 
			{
				return 0;
			}

			TranslateMessage(&currentMsg);
			DispatchMessage(&currentMsg);
		}
		return 1;
	}
	void Application::Init()
	{
		// TODO: initialize dx12 related stuff here and imgui

		IMGUI_CHECKVERSION();

	}
	void Application::Shutdown()
	{
		// ImGui
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void Application::DrawEditorLayers()
	{
		for (auto& layer : m_editorLayers)
		{
			layer->DrawLayer();
		}
	}
}