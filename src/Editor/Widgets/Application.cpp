#include "Application.h"

#include <stdexcept>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

#include <dxgi1_4.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

struct FrameContext
{
	ID3D12CommandAllocator*         CommandAllocator;
	UINT64                          FenceValue;
};

// DX12 Global
static int const                    NUM_BACK_BUFFERS = 3;

// TODO: implement setting up d3d devices
static void SetupDX()
{

}

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

		// Initialization
		Init();

		// Showing of the window
		ShowWindow(hWnd, SW_SHOW);
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

		// Rendering, ImGuiContext
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		renderer.BeginFrame();
		ID3D12GraphicsCommandList6* curCommandList = renderer.GetCurrentCommandList();

		ImGui::ShowDemoWindow();

		// Layers -> Drawing
		DrawEditorLayers();
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), curCommandList);

		renderer.EndFrame();
		renderer.Render();

		return true;
	}

	float Application::GetTimeStamp()
	{
		// TODO: implement, we might need to look for dx12 related timestamps/frequency here...
		return 0.0f;
	}

	HINSTANCE Application::GetInstance() const
	{
		return hInstance;
	}

	LRESULT CALLBACK Application::AppProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND ig_hWnd, UINT ig_msg, WPARAM ig_wParam, LPARAM ig_lParam);
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
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
		renderer.InitDescriptorHeaps(1024, 32, 256, 256);

		RECT rect;
		if (!GetClientRect(hWnd, &rect))
			throw std::exception("Couldn't get window size");
		
		renderer.InitSwapChain(rect.right - rect.left, rect.bottom - rect.top, NUM_BACK_BUFFERS, hWnd);
		renderer.InitCommandFrames();
		renderer.InitFrameFence();
		renderer.CreateRootSignature();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Style
		// TODO: implement the reckless editor style later here...
		ImGui::StyleColorsDark();

		// Setting of the ImGui platform and renderer
		ImGui_ImplWin32_Init(hWnd);
		D3D12::DescriptorHeapHandle font_descriptor_handle = renderer.cbv_srv_uav_descHeap.Allocate();
		ImGui_ImplDX12_Init(
			renderer.api.GetDevicePtr().Get(), 
			NUM_BACK_BUFFERS, 
			DXGI_FORMAT_R8G8B8A8_UNORM, //TODO: Make this always match definition in renderer
			renderer.cbv_srv_uav_descHeap.GetHeapPtr().Get(), 
			font_descriptor_handle.cpuHandle, font_descriptor_handle.gpuHandle
		);
	}
	void Application::Shutdown()
	{
		// ImGui
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		// Wait for renderer to finish up
		renderer.Shutdown();
	}
	void Application::DrawEditorLayers()
	{
		for (auto& layer : m_editorLayers)
		{
			layer->DrawLayer();
		}
	}
}