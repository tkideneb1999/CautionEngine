#pragma once

#include <CautionDefinitions.h>

#include "D3D12API.h"
#include "DescriptorManager.h"
#include "RenderTarget.h"

#include <vector>

namespace CautionEngine::Rendering 
{
	struct CAUTION_CLASS CommandFrame
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		UINT64 fenceValue;
	};

	class CAUTION_CLASS Renderer
	{
	public:
		static D3D12API s_api;

		DescriptorManager descriptorManager;

		unsigned int numBackBuffers;

	private:
		std::vector<RenderTarget> m_swapChainRenderTargets;

		ComPtr<ID3D12CommandQueue> m_commandQueue;

		ComPtr<IDXGISwapChain4> m_swapChain;
		DXGI_FORMAT m_swapChainRTFormat = DXGI_FORMAT_UNKNOWN;

		std::vector<CommandFrame> m_commandFrames;
		// TODO: Possibility for multiple command lists per Command Frame
		std::vector<ComPtr<ID3D12GraphicsCommandList6>> m_commandLists;

		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12PipelineState> m_initialPipelineState;
		D3D12_VIEWPORT m_viewport = {};
		D3D12_RECT m_scissorRect = {};

		// Synchronization Objects
		UINT m_curFrameIndex = 0;
		HANDLE m_fenceEvent;
		UINT64 m_fenceValue = 0;
		ComPtr<ID3D12Fence> m_fence;

	public:
		Renderer();
		virtual ~Renderer() = default;

		void Render();

		void InitSwapChain(int width, int height, int frameCount, HWND hWnd);
		void InitDescriptorHeaps(int cbv_srv_uav_count, int dsv_count, int rtv_count, int sampler_count);
		void InitCommandFrames();
		void InitFrameFence();
		void BeginFrame();
		void EndFrame();
		void CreateRootSignature();
		void CreateInitialPipelineState();
		void Shutdown();
		void FlushGPU();

		void Resize(int newWidth, int newHeight);
		void CreateSwapChainRenderTargets();
		void ReleaseSwapChainRenderTargets();

		ID3D12GraphicsCommandList6* GetCurrentCommandList(); // TODO: Wrap Command List somehow
		DXGI_FORMAT GetRTVFormat();
	};
}