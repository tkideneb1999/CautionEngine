#include "Application.h"

#include "../Platform/ImGui/RecklessEdTheme.h"


#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

#include <dxgi1_6.h>
#include <wrl/client.h>

#include <stdexcept>
#include <iostream>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

// DX12 Global
static int const                    NUM_BACK_BUFFERS = 3;

namespace Reckless
{
	CWinApplication::CWinApplication(const wchar_t wndClassName[], const wchar_t wndName[], std::vector<std::string> lArgs)
		: args(lArgs)
	{
		hInstance = GetModuleHandle(nullptr);

		windowClass = { 0 };
		windowClass.lpfnWndProc = AppProcedureSetup;
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
			this
		);

		if (hWnd == nullptr) {
			throw std::runtime_error("Error Initializing Window");
		}
		UpdateWindowSize();

		// Initialization
		Initialize();

		// Showing of the window
		ShowWindow(hWnd, SW_SHOW);
	}

	CWinApplication::~CWinApplication()
	{
		Shutdown();
	}

	float CWinApplication::GetTimeStamp()
	{
		// TODO: implement, we might need to look for dx12 related timestamps/frequency here...
		return 0.0f;
	}

	const HINSTANCE* CWinApplication::GetInstance() const
	{
		return &hInstance;
	}

	LRESULT CWinApplication::AppProcedureSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			CWinApplication* const pApp = static_cast<CWinApplication*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWinApplication::AppProcedure));
			return pApp->HandleMessage(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK CWinApplication::AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND ig_hWnd, UINT ig_msg, WPARAM ig_wParam, LPARAM ig_lParam);
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
		CWinApplication* const pApp = reinterpret_cast<CWinApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pApp->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT CWinApplication::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_SIZE:
			UpdateWindowSize();
			m_renderer.Resize(m_width, m_height);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	int CWinApplication::HandleMessages()
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

	void CWinApplication::Initialize()
	{
		// TODO: initialize dx12 related stuff here and imgui
		m_renderer.InitDescriptorHeaps(1024, 32, 256, 256);

		RECT rect;
		if (!GetClientRect(hWnd, &rect))
			throw std::exception("Couldn't get window size");
		
		m_renderer.InitSwapChain(m_width, m_height, NUM_BACK_BUFFERS, hWnd);
		m_renderer.InitCommandFrames();
		m_renderer.InitFrameFence();
		m_renderer.CreateRootSignature();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		// IO
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Docking
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // TODO: benedikt -> you might be interested in this feature of dear imgui

		// Menu
		io.ConfigFlags |= ImGuiWindowFlags_MenuBar;

		// Reckless Style
		Reckless::UI::SetRecklessEdTheme();

		// ImGui Styling
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.FramePadding = ImVec2(8.0f, 6.0f);
		style.ItemSpacing = ImVec2(6.0f, 6.0f);
		style.FrameRounding = 8.0f;
		style.WindowTitleAlign = ImVec2(1.f, 1.f);
		style.WindowMinSize.y = 1080.f;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setting of the ImGui platform and renderer
		ImGui_ImplWin32_Init(hWnd);
		DescriptorManager* descriptorManager = &m_renderer.descriptorManager;
		D3D12::DescriptorHeapHandle font_descriptor_handle = descriptorManager->GetCbvSrvUavHeap()->Allocate();
		ImGui_ImplDX12_Init(
			m_renderer.s_api.GetDevicePtr().Get(), 
			NUM_BACK_BUFFERS, 
			DXGI_FORMAT_R8G8B8A8_UNORM, //TODO: Make this always match definition in renderer
			descriptorManager->GetCbvSrvUavHeap()->GetHeapPtr().Get(),
			font_descriptor_handle.cpuHandle, font_descriptor_handle.gpuHandle
		);
	}

	void CWinApplication::Run()
	{
		m_running = true;

		while (IsWindow(hWnd) && m_running)
			Update();
	}

	void CWinApplication::Close()
	{
		m_running = false;
	}

	bool CWinApplication::Update()
	{
		ImGuiIO& io = ImGui::GetIO();

		// Handle Messages
		if (HandleMessages() != 1)
		{
			// Do Shutdown here
			return false;
		}

		// Rendering, ImGuiContext
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ID3D12GraphicsCommandList6* curCommandList = m_renderer.GetCurrentCommandList();

		ImGui::NewFrame();

		{
			m_renderer.BeginFrame();
			

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
		/*	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);*/

			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			window_flags |= ImGuiWindowFlags_MenuBar;

			/*ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);*/

			//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

			ImGui::Begin("DockSpaceWindow", nullptr, window_flags);

			// Docking
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowMinSize.x = 1080.f;
			ImGui::DockSpace(ImGui::GetID("MyDockspace"));

			// Layers -> Drawing
			UpdateEditorLayers();

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), curCommandList);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		m_renderer.EndFrame();
		m_renderer.Render();

		return true;
	}

	void CWinApplication::Shutdown()
	{
		m_running = false;

		for (const auto& layer : m_editorLayers)
		{
			layer->OnEditorLayerDetach();
		}

		m_editorLayers.clear();

		// ImGui
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		// Wait for renderer to finish up
		m_renderer.Shutdown();
	}

	void CWinApplication::UpdateEditorLayers()
	{
		for (const auto& layer : m_editorLayers)
		{
			layer->Update();
		}
	}

	void CWinApplication::UpdateWindowSize()
	{
		RECT rect;
		if (!GetClientRect(hWnd, &rect))
			throw std::exception("Couldn't get window size");
		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;
	}
}