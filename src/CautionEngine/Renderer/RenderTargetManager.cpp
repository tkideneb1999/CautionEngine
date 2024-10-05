#include "stdafx.h"
#include "RenderTargetManager.h"

#include "Renderer.h"
#include "D3D12API.h"
#include "D3D12Helpers.h"

#include <limits>

namespace CautionEngine::Rendering 
{
	RenderTargetManager::RenderTargetManager(DescriptorManager* pDescriptorManager)
	{
		m_pDescriptorManager = pDescriptorManager;
	}

	unsigned int RenderTargetManager::CreateRenderTarget(
		unsigned int width, unsigned int height, RenderFormat format, 
		unsigned int mipLevel, const float* clearValue
	)
	{
		unsigned int id = GetFreeID();
		RenderTarget rt = RenderTarget();
		rt.m_width = width;
		rt.m_height = height;
		rt.m_id = id;

		if (format == RENDER_FORMAT_D32_FLOAT)
		{
			// TODO
			CreateRenderTargetResource(
				width, height, format, mipLevel, true,
				nullptr, rt.pResource
			);
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateDSV();
			D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_D32_FLOAT;
			desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			desc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12API::Get()->GetDevicePtr()->CreateDepthStencilView(
				rt.pResource.Get(), &desc, rt.descriptorHeapHandle.cpuHandle
			);
		}
		else
		{
			// TODO: Make this prettier
			if (clearValue == nullptr)
			{
				float clearValue[] = { 0.0, 0.0, 0.0, 1.0 };
				CreateRenderTargetResource(
					width, height, format, mipLevel, false, 
					&clearValue[0], rt.pResource
				);
			}
			else
			{
				CreateRenderTargetResource(
					width, height, format, mipLevel, false, 
					clearValue, rt.pResource
				);
			}
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateRTV();
			D3D12_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = (DXGI_FORMAT)format;
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;
			D3D12API::Get()->GetDevicePtr()->CreateRenderTargetView(
				rt.pResource.Get(), &desc, rt.descriptorHeapHandle.cpuHandle
			);
			m_renderTargets.insert(std::pair<unsigned int, RenderTarget>(id, rt));
		}
		
		return id;
	}

	RenderTarget* RenderTargetManager::GetRenderTarget(unsigned int id)
	{
		auto res = m_renderTargets.find(id);
		if (res == m_renderTargets.end())
		{
			return nullptr;
		}
		return &(res->second);
	}

	void RenderTargetManager::Shutdown()
	{
		for (auto elem : m_renderTargets)
		{
			elem.second.Release();
		}
		m_renderTargets.clear();
	}

	void RenderTargetManager::CreateRenderTargetResource(
		unsigned int width, unsigned int height, RenderFormat format, unsigned int mipLevels, bool isDepthRT,
		const float* pClearValue, Microsoft::WRL::ComPtr<ID3D12Resource>& pRenderTargetResource
	)
	{
		// TODO: This needs to be more generic
		D3D12_HEAP_PROPERTIES heapProps;
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 0;
		heapProps.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC renderTargetDesc;
		renderTargetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		renderTargetDesc.Alignment = 0; // TODO: Look into optimizing this
		renderTargetDesc.Width = width;
		renderTargetDesc.Height = height;
		renderTargetDesc.DepthOrArraySize = 1;
		renderTargetDesc.MipLevels = mipLevels;
		renderTargetDesc.Format = (DXGI_FORMAT)format;

		// MSAA TODO
		renderTargetDesc.SampleDesc.Count = 1;
		renderTargetDesc.SampleDesc.Quality = 0;

		renderTargetDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // TODO: Figure out
		if (isDepthRT)
		{
			renderTargetDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}
		else
		{
			renderTargetDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}

		D3D12_CLEAR_VALUE clearValue;
		// TODO Depth Stencil Textures
		if (isDepthRT)
		{
			clearValue.Format = (DXGI_FORMAT)format;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;
		}
		else
		{
			clearValue.Format = (DXGI_FORMAT)format;
			for (UINT8 i = 0; i < 4; ++i)
			{
				clearValue.Color[i] = pClearValue[i];
			}
		}

		D3D12_HEAP_FLAGS heapFlags = isDepthRT == false ? D3D12_HEAP_FLAG_ALLOW_DISPLAY : D3D12_HEAP_FLAG_NONE;
		D3D12_RESOURCE_STATES initialState = isDepthRT == false ? D3D12_RESOURCE_STATE_COMMON : D3D12_RESOURCE_STATE_DEPTH_WRITE;

		THROW_IF_FAILED(
			D3D12API::Get()->GetDevicePtr()->CreateCommittedResource(
				&heapProps,
				heapFlags,
				&renderTargetDesc,
				initialState,
				&clearValue,
				IID_PPV_ARGS(&pRenderTargetResource)
			),
			"Could not create Rendertarget"
		);
	}

	unsigned int RenderTargetManager::GetFreeID()
	{
		for (unsigned int id = 0; id < std::numeric_limits<unsigned int>::max(); ++id)
		{
			if (m_renderTargets.find(id) == m_renderTargets.end())
			{
				return id;
			}
		}

		throw std::exception("A free ID could not be generated");
	}
}
