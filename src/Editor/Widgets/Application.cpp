#include "Application.h"

#include <stdexcept>

namespace Reckless {
	Application::Application(const wchar_t wndClassName[], const wchar_t wndName[], std::vector<std::string> lArgs)
		:args{ lArgs }
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

		return true;
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
}