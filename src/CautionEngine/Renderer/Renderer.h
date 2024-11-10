#pragma once

#include <CautionDefinitions.h>

#include "D3D12API.h"
#include "DescriptorManager.h"
#include "RenderTarget.h"
#include "ShaderManager.h"

#include <vector>

#include "Mesh.h"
#include "PipelineStateObject.h"

namespace CautionEngine::Rendering 
{
	class ConstantBufferManager;

	struct CAUTION_CLASS CommandFrame
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		UINT64 fenceValue;
	};

	class CAUTION_CLASS Renderer
	{
	public:
		D3D12API* const pD3D12API; // TODO: Make this private

		unsigned int numBackBuffers;

	private:
		RenderTargetManager* m_pRenderTargetManager = nullptr;
		DescriptorManager* m_pDescriptorManager = nullptr;
		ConstantBufferManager* m_pConstantBufferManager = nullptr;
		ShaderManager* m_pShaderManager;

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

		// Custom Scene Render Target
		bool m_useCustomSceneRenderTarget = false;
		unsigned int m_customRenderTargetId;
		D3D12_VIEWPORT m_customRTViewPort = {};
		D3D12_RECT m_customRTScissorRect = {};

		// Synchronization Objects
		UINT m_curFrameIndex = 0;
		HANDLE m_fenceEvent;
		UINT64 m_fenceValue = 0;
		ComPtr<ID3D12Fence> m_fence;

	public:
		Renderer();
		virtual ~Renderer() = default;

		void Render();

		static ID3D12Device* GetD3D12DevicePtr() { return D3D12API::Get()->GetDevicePtr().Get(); }
		void InitSwapChain(int width, int height, int frameCount, HWND hWnd);
		void InitDescriptorHeaps(int cbv_srv_uav_count, int dsv_count, int rtv_count, int sampler_count);
		void InitCommandFrames();
		void InitFrameFence();
		void BeginFrame();
		void RenderScene(); // TODO: Put Scene Reference here
		void EndFrame();
		void CreateRootSignature();
		void Shutdown();
		void FlushGPU();

		void Resize(int newWidth, int newHeight);
		void CreateSwapChainRenderTargets();
		void ReleaseSwapChainRenderTargets();

		ID3D12GraphicsCommandList6* GetCurrentCommandList(); // TODO: Wrap Command List somehow
		DXGI_FORMAT GetRTVFormat(); // TODO: Convert this to 'RenderFormat'

		DescriptorManager* GetDescriptorManager() const { return m_pDescriptorManager; }
		RenderTargetManager* GetRenderTargetManager() const { return m_pRenderTargetManager; }

		void SetCustomSceneRenderTarget(unsigned int customRenderTargetId);
		void DisableCustomSceneRenderTarget();
	};
}