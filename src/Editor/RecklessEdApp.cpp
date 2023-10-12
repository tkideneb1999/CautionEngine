#include <Windows.h>
#include <iostream>

bool g_applicationRunning = true;
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";

LRESULT CALLBACK EditorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) 
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void main(int argc, char** argv)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASS editorWindowClass = {0};
	editorWindowClass.lpfnWndProc = EditorProc;
	editorWindowClass.hInstance = hInstance;
	editorWindowClass.lpszClassName = CLASS_NAME;

	RegisterClass(&editorWindowClass);

	HWND hEditorHandle = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Reckless Editor",
		WS_OVERLAPPEDWINDOW, // Window Style

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Position & Size

		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hEditorHandle == nullptr)
	{
		return;
	}

	ShowWindow(hEditorHandle, SW_SHOW);

	MSG msg;
	BOOL msgResult;
	while ((msgResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		// TODO: application running here...
	}

	if (msgResult == -1) {
		std::cout << "Error Occurred" << std::endl;
	}
}