#include "stdafx.h"

// Reckless
#include "RecklessEdCoreApi.h"

// CautionEngine
#include <Renderer/D3D12API.h>

// configs
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";
const wchar_t WINDOW_NAME[] = L"RecklessEd";

using namespace Reckless;

extern CWinApplication* s_recklessEditor;


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
	// TODO: make proper initialization of the RecklessApplication
	s_recklessEditor = new CWinApplication(CLASS_NAME, WINDOW_NAME, args);

	// AssetBrowser Editor
	CAssetBrowserEditorSharedPtr pAssetBrowserLayer = std::make_shared<CAssetBrowserEditor>();
	GetRecklessEditor()->AddEditorLayer(pAssetBrowserLayer);

	// Viewport
	CEditorViewportSharedPtr pViewportLayer = std::make_shared<CEditorViewport>();
	GetRecklessEditor()->AddEditorLayer(pViewportLayer);

	// Toolbar
	CMainEditorToolbarSharedPtr pToolbar = std::make_shared<CMainEditorToolbar>();
	GetRecklessEditor()->AddEditorLayer(pToolbar);

	// Properties
	CProperyEditorSharedPtr pProperties = std::make_shared<CProperyEditor>();
	GetRecklessEditor()->AddEditorLayer(pProperties);

	s_recklessEditor->Run();
	delete s_recklessEditor;
	return 0;
}