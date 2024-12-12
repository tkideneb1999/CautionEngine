#include "stdafx.h"

// Reckless
#include "RecklessEdCoreApi.h"

// CautionEngine
#include <Renderer/D3D12API.h>
#include <Core/Entity.h>

// configs
const wchar_t CLASS_NAME[] = L"Reckless Editor Class";
const wchar_t WINDOW_NAME[] = L"RecklessEd";

using namespace Reckless;

extern CWinApplication* s_recklessEditor;


int main(int argc, char** argv)
{
	std::vector<std::string> args(argc);
	for (size_t i = 0; i < argc; ++i)
	{
		args[i] = argv[i];
	}

	// API needs to be initialized separately, because Engine is a DLL
	// https://gamedev.stackexchange.com/questions/128197/why-do-i-get-this-error-about-dllmain-when-using-d3d-from-within-a-dll
	// TODO: Move Init and Shutdown of D3D12API out of here
	CautionEngine::Rendering::InitD3D12API();
	
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

	// TEST Entity Generation
	using namespace CautionEngine;
	std::vector<std::shared_ptr<Core::CEntity>> m_entities;
	for (size_t i = 0; i < 10; ++i)
	{
		Core::CEntity* pEntity = new Core::CEntity("Entity Sample");
		m_entities.emplace_back(pEntity);
	}


	s_recklessEditor->Run();
	m_entities.clear();
	delete s_recklessEditor;
	CautionEngine::Rendering::ShutdownD3D12API();
	return 0;
}
