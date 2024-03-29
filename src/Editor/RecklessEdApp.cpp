#include <vector>
#include <string>

#include "Widgets/Application.h"
#include "Widgets/RecklessEditors.h"

#include <Renderer/D3D12API.h>

// configs
bool g_applicationRunning = true;
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";
const wchar_t WINDOW_NAME[] = L"Reckless Editor";


int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++) 
	{
		args[i] = argv[i];
	}
	
	// API needs to be initialized separately, because Engine is a DLL
	// https://gamedev.stackexchange.com/questions/128197/why-do-i-get-this-error-about-dllmain-when-using-d3d-from-within-a-dll
	CautionEngine::Rendering::Renderer::s_api.Init();
	Reckless::Application* editor = new Reckless::Application(CLASS_NAME, WINDOW_NAME, args);

	// AssetBrowser Editor
	std::shared_ptr<Reckless::AssetBrowserEditor> assetBrowser = std::make_shared<Reckless::AssetBrowserEditor>();
	editor->AddEditorLayer(assetBrowser);

	// Viewport
	

	while (g_applicationRunning)
	{
		if (!editor->Update())
			return 0;
	}

	delete editor;
	return 0;
}