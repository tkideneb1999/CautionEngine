#pragma once

#include <vector>

#include "D3D12API.h"
#include "D3D12DescriptorHeap.h"
#include "RenderTarget.h"

namespace CautionEngine::Rendering {
	struct CommandFrame {
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		UINT64 fenceValue;
	};

	class Renderer {

	public:
		static D3D12API api;

		D3D12::DescriptorHeap cbv_srv_uav_descHeap;
		D3D12::DescriptorHeap rtv_descHeap;
		D3D12::DescriptorHeap dsv_descHeap;
		D3D12::DescriptorHeap sampler_descHeap;

		unsigned int numBackBuffers;

	private:
		std::vector<RenderTarget> m_swapChainRenderTargets;

		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<IDXGISwapChain4> m_swapChain;

		std::vector<CommandFrame> m_commandFrames;
		// TODO: Possibility for multiple command lists per Command Frame
		std::vector<ComPtr<ID3D12GraphicsCommandList6>> m_commandLists;

		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12PipelineState> m_initialPipelineState;
		D3D12_VIEWPORT m_viewport = {};
		D3D12_RECT m_scissorRect = {};

		// Synchronization Objects: TODO
		UINT m_curFrameIndex = 0;
		HANDLE m_fenceEvent;
		UINT64 m_fenceValue = 0;
		ComPtr<ID3D12Fence> m_fence;

	public:
		Renderer();
		~Renderer();

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

		ID3D12GraphicsCommandList6* GetCurrentCommandList();
	};
}