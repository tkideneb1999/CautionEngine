#include "Widgets/Application.h"
#include "Widgets/RecklessEditors.h"

// CautionEngine
#include <Renderer/D3D12API.h>

// CautionCommon
#include <Platform/CautionPlatform.h>

// STL
#include <vector>
#include <string>

// configs
bool g_applicationRunning = true;
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";
const wchar_t WINDOW_NAME[] = L"Reckless Editor";

// TODO: johne -> make global editor getter

int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++) 
	{
		args[i] = argv[i];
	}
	
	using namespace Reckless;
	// API needs to be initialized separately, because Engine is a DLL
	// https://gamedev.stackexchange.com/questions/128197/why-do-i-get-this-error-about-dllmain-when-using-d3d-from-within-a-dll
	CautionEngine::Rendering::Renderer::s_api.Init();
	Application* const pRecklesssEd = new Application(CLASS_NAME, WINDOW_NAME, args);

	// AssetBrowser Editor
	AssetBrowserEditorSharedPtr pAssetBrowserLayer = std::make_shared<AssetBrowserEditor>();
	pRecklesssEd->AddEditorLayer(pAssetBrowserLayer);

	// Viewport
	EditorViewportSharedPtr pViewportLayer = std::make_shared<EditorViewport>();
	pRecklesssEd->AddEditorLayer(pViewportLayer);

	// Toolbar
	ToolbarSharedPtr pToolbar = std::make_shared<Toolbar>();
	pRecklesssEd->AddEditorLayer(pToolbar);
	

	// TEST
	ToolbarSharedPtr sample = pRecklesssEd->GetEditorLayer<Toolbar>();
	sample->AddMenu("test", [] {});
	

	while (g_applicationRunning)
	{
		if (!pRecklesssEd->Update())
			return 0;
	}

	delete pRecklesssEd;
	return 0;
}