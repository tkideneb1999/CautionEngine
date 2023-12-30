#include "Renderer.h"

#include <string>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering {

	D3D12API Renderer::api = {};

	Renderer::Renderer()
	{
		
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Render()
	{
		
		// Present SwapChain
		HRESULT res = m_swapChain->Present(1, 0);
		if (!SUCCEEDED(res))
		{
			if(res == DXGI_ERROR_DEVICE_REMOVED)
				api.GatherDREDOUTput();
		}
		CommandFrame& curFrame = m_commandFrames[m_curFrameIndex];
		m_fenceValue++;
		curFrame.fenceValue = m_fenceValue;
		m_commandQueue->Signal(m_fence.Get(), m_fenceValue);


		m_curFrameIndex = (m_curFrameIndex + 1) % numBackBuffers;
		//m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();
	}

	void Renderer::InitSwapChain(int width, int height, int frameCount, HWND hWnd)
	{
		//Check if RTV Descriptor Heap is initialized
		if (!rtv_descHeap.Initialized())
			throw std::exception("RTV Descriptor Heap not initialized");

		numBackBuffers = frameCount;

		// Init Command Queue
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(
			api.GetDevicePtr()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue)),
			"Command Queue Creation Failed"
			);
		m_commandQueue->SetName(L"Renderer Command Queue");

		// Init SwapChain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = frameCount;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Stereo = false;

		swapChainDesc.Width = width;
		swapChainDesc.Height = height;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};
		swapChainFullScreenDesc.Windowed = true;

		ComPtr<IDXGISwapChain1> i_swapChain;
		ThrowIfFailed(
			api.GetFactoryPtr()->CreateSwapChainForHwnd(m_commandQueue.Get(), hWnd, &swapChainDesc, &swapChainFullScreenDesc, nullptr, i_swapChain.GetAddressOf()),
			"Swap Chain Creation Failed"
			);
		ThrowIfFailed(i_swapChain.As(&m_swapChain), "Cast to Swap Chain Failed");

		//m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

		//Init Swap Chain RTV Resources
		m_swapChainRenderTargets.clear();
		m_swapChainRenderTargets.reserve(frameCount);

		for (int i = 0; i < frameCount; i++)
		{
			ComPtr<ID3D12Resource> renderTarget;
			m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget));
			renderTarget->SetName((std::wstring(L"SwapChain Render Target ") + std::to_wstring(i)).c_str());
			D3D12::DescriptorHeapHandle swapChainBufferHandle = rtv_descHeap.Allocate();
			m_swapChainRenderTargets.push_back(RenderTarget(swapChainBufferHandle, renderTarget));
			api.GetDevicePtr()->CreateRenderTargetView(
				m_swapChainRenderTargets[i].resourceView.Get(), nullptr, m_swapChainRenderTargets[i].descriptorHeapHandle.cpuHandle
			);
		}

		//TODO: Put this somewhere else
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Height = height;
		m_viewport.Width = width;
		m_viewport.MinDepth = 0.0;
		m_viewport.MaxDepth = 1.0;

		m_scissorRect.left = 0;
		m_scissorRect.top = 0;
		m_scissorRect.right = width;
		m_scissorRect.bottom = height;

	}

	void Renderer::InitDescriptorHeaps(int cbv_srv_uav_count, int dsv_count, int rtv_count, int sampler_count)
	{
		cbv_srv_uav_descHeap = D3D12::DescriptorHeap(api.GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, cbv_srv_uav_count, true);
		dsv_descHeap = D3D12::DescriptorHeap(api.GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, dsv_count, true);
		rtv_descHeap = D3D12::DescriptorHeap(api.GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rtv_count, true);
		sampler_descHeap = D3D12::DescriptorHeap(api.GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, sampler_count, true);
	}

	void Renderer::InitCommandFrames()
	{
		//api.GetDevicePtr()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));

		m_commandFrames.reserve(numBackBuffers);
		m_commandLists.reserve(numBackBuffers);

		for (int i = 0; i < numBackBuffers; i++)
		{
			m_commandFrames.push_back(CommandFrame());
			m_commandFrames[i].fenceValue = 0;
			
			ThrowIfFailed(
				api.GetDevicePtr()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&(m_commandFrames[i].commandAllocator))),
				"Command Allocator Creation Failed"
			);

			// TODO: Remove this
			m_commandLists.push_back(ComPtr<ID3D12GraphicsCommandList6>());
			ThrowIfFailed(
				api.GetDevicePtr()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&(m_commandLists[i]))),
				"Command List Creation Failed"
			);
		}
	}

	ID3D12GraphicsCommandList6* Renderer::GetCurrentCommandList()
	{
		return m_commandLists[m_curFrameIndex].Get();
	}

	void Renderer::InitFrameFence()
	{
		ThrowIfFailed(
			api.GetDevicePtr()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)),
			"Fence creation failed"
		);
		m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
		if (m_fenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()), "Fence Event Creation Failed");
		}
	}

	void Renderer::BeginFrame()
	{
		CommandFrame& curFrame = m_commandFrames[m_curFrameIndex];

		// Check if Frame is finished Rendering here
		UINT64 completedFenceValue = m_fence->GetCompletedValue();
		if (completedFenceValue < curFrame.fenceValue)
		{
			m_fence->SetEventOnCompletion(curFrame.fenceValue, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		curFrame.commandAllocator->Reset();

		ComPtr<ID3D12GraphicsCommandList6>& curCommandList = m_commandLists[m_curFrameIndex];
		curCommandList->Reset(m_commandFrames[m_curFrameIndex].commandAllocator.Get(), nullptr);
		curCommandList->OMSetRenderTargets(1, &(m_swapChainRenderTargets[m_curFrameIndex].descriptorHeapHandle.cpuHandle), false, nullptr);

		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = m_swapChainRenderTargets[m_curFrameIndex].GetPtr().Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		curCommandList->ResourceBarrier(1, &barrier);

		ID3D12DescriptorHeap* ppShaderVisibleHeaps[] = {cbv_srv_uav_descHeap.GetHeapPtr().Get(), sampler_descHeap.GetHeapPtr().Get()};

		curCommandList->SetDescriptorHeaps(_countof(ppShaderVisibleHeaps), ppShaderVisibleHeaps);
		curCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
		curCommandList->RSSetViewports(1, &m_viewport);
		curCommandList->RSSetScissorRects(1, &m_scissorRect);

		float color[] = { 0.2, 0.2, 0.3, 1.0 };
		m_commandLists[m_curFrameIndex]->ClearRenderTargetView(
			m_swapChainRenderTargets[m_curFrameIndex].descriptorHeapHandle.cpuHandle, color, 0, nullptr
		);

		// TODO: Command List Synchronization
	}

	void Renderer::EndFrame()
	{
		ComPtr<ID3D12GraphicsCommandList6>& curCommandList = m_commandLists[m_curFrameIndex];

		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = m_swapChainRenderTargets[m_curFrameIndex].GetPtr().Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		curCommandList->ResourceBarrier(1, &barrier);

		curCommandList->Close();
		// Execute Command List
		ID3D12CommandList* ppCommandLists[] = { curCommandList.Get() };
		m_commandQueue->ExecuteCommandLists(1, ppCommandLists);
	}

	void Renderer::CreateRootSignature()
	{
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.NumParameters = 0;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;

		HRESULT res = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		ThrowIfFailed(res, "Root Signature Serialization failed");
		ThrowIfFailed(
			api.GetDevicePtr()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)),
			"Couldn't create Root Signature"
		);
	}

	void Renderer::CreateInitialPipelineState()
	{

	}

	void Renderer::Shutdown()
	{
		FlushGPU();

		CloseHandle(m_fenceEvent);
	}

	void Renderer::FlushGPU()
	{
		for (int i = 0; i < numBackBuffers; i++)
		{
			CommandFrame& frame = m_commandFrames[i];
			UINT64 completedFenceValue = m_fence->GetCompletedValue();
			if (completedFenceValue < frame.fenceValue)
			{
				m_fence->SetEventOnCompletion(frame.fenceValue, m_fenceEvent);
				WaitForSingleObject(m_fenceEvent, INFINITE);
			}
		}
		m_curFrameIndex = 0;
	}

	void Renderer::Resize(int newWidth, int newHeight)
	{
		FlushGPU();
		ReleaseSwapChainRenderTargets();
		m_swapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
		CreateSwapChainRenderTargets();
	}

	void Renderer::CreateSwapChainRenderTargets()
	{
		for (int i = 0; i < numBackBuffers; i++)
		{
			RenderTarget& renderTarget = m_swapChainRenderTargets[i];
			m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget.resourceView));
			api.GetDevicePtr()->CreateRenderTargetView(
				renderTarget.resourceView.Get(), nullptr, renderTarget.descriptorHeapHandle.cpuHandle
			);
		}
	}

	void Renderer::ReleaseSwapChainRenderTargets()
	{
		for (int i = 0; i < numBackBuffers; i++)
		{
			RenderTarget& renderTarget = m_swapChainRenderTargets[i];
			renderTarget.resourceView.Reset();
		}
	}
}
