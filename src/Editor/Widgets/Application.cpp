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

// DX12 Globals
static int const                    NUM_FRAMES_IN_FLIGHT = 3;
static FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
static UINT                         g_frameIndex = 0;

static int const                    NUM_BACK_BUFFERS = 3;
static ID3D12Device*                g_pd3dDevice = NULL;
static ID3D12DescriptorHeap*        g_pd3dRtvDescHeap = NULL;
static ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = NULL;
static ID3D12CommandQueue*          g_pd3dCommandQueue = NULL;
static ID3D12GraphicsCommandList*   g_pd3dCommandList = NULL;
static ID3D12Fence*                 g_fence = NULL;
static HANDLE                       g_fenceEvent = NULL;
static UINT64                       g_fenceLastSignaledValue = 0;
static IDXGISwapChain3*             g_pSwapChain = NULL;
static HANDLE                       g_hSwapChainWaitableObject = NULL;
static ID3D12Resource*              g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

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
		// Layers -> Drawing
		DrawEditorLayers();
		ImGui::Render();

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
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Style
		// TODO: implement the reckless editor style later here...
		ImGui::StyleColorsDark();

		// Setting of the ImGui platform and renderer
		ImGui_ImplWin32_Init(hWnd);
		// DX12 Info
		ImGui_ImplDX12_Init
		(g_pd3dDevice
			, NUM_FRAMES_IN_FLIGHT
			, DXGI_FORMAT_R8G8B8A8_UNORM
			, g_pd3dSrvDescHeap
			, g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart()
			, g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart()
		);
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