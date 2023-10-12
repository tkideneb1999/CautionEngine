#pragma once

#include <Windows.h>
#include <vector>
#include <string>

#include "imgui.h"

namespace Reckless
{
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

	private:
		static LRESULT CALLBACK AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int HandleMessages();
	};
}