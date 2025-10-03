#include "stdafx.h"
#include "RenderTargetManager.h"

#include "Renderer.h"
#include "D3D12API.h"
#include "D3D12Helpers.h"

#include <limits>

namespace CautionEngine::Rendering 
{
	RenderTargetManager::RenderTargetManager(std::shared_ptr<DescriptorManager> pDescriptorManager)
	{
		m_pDescriptorManager = pDescriptorManager;
	}

	unsigned int RenderTargetManager::CreateRenderTarget(
		unsigned int width, unsigned int height, RenderFormat format, 
		unsigned int mipLevel, const float* clearValue
	)
	{
		unsigned int id = GetFreeID();
		bool isDepthRT = format == RENDER_FORMAT_D32_FLOAT;
		float finalClearValue[4]{};
		if (clearValue == 0)
		{
			if (isDepthRT)
				finalClearValue[0] = 1.0f;
			else
			{
				finalClearValue[0] = 0.0f;
				finalClearValue[1] = 0.0f;
				finalClearValue[2] = 0.0f;
				finalClearValue[3] = 1.0f;
			}
		}

		RenderTarget rt = RenderTarget(format, width, height, id, mipLevel, finalClearValue);
		CreateRenderTargetResource(
			width, height, format, mipLevel, isDepthRT,
			finalClearValue, rt.pResource
		);

		if (isDepthRT)
		{
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateDSV();
			CreateDepthStencilView(rt);
		}
		else
		{
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateRTV();
			CreateRenderTargetView(rt);
		}
		m_renderTargets.insert(std::pair<unsigned int, RenderTarget>(id, rt));
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

	void RenderTargetManager::ReleaseRenderTarget(unsigned int id)
	{
		auto res = m_renderTargets.find(id);
		if (res == m_renderTargets.end())
		{
			return;
		}
		res->second.Release();
		m_renderTargets.erase(id);
	}

	void RenderTargetManager::ResizeRenderTarget(unsigned int id, unsigned int newWidth, unsigned int newHeight)
	{
		auto res = m_renderTargets.find(id);
		if (res == m_renderTargets.end())
		{
			return;
		}
		RenderTarget& rt = res->second;
		rt.pResource = ComPtr<ID3D12Resource>{};
		CreateRenderTargetResource(newWidth, newHeight, 
			rt.m_renderFormat, rt.m_mipLevels, rt.m_isDepthRT, rt.m_clearValue, rt.pResource);
		if (rt.m_isDepthRT)
		{
			CreateDepthStencilView(rt);
		}
		else
		{
			CreateRenderTargetView(rt);
		}
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
			clearValue.DepthStencil.Depth = *pClearValue;
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
		D3D12_RESOURCE_STATES initialState = isDepthRT == false ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_DEPTH_WRITE;

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

	void RenderTargetManager::CreateRenderTargetView(RenderTarget& rt)
	{
		D3D12_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = (DXGI_FORMAT)rt.m_renderFormat;
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		desc.Texture2D.PlaneSlice = 0;
		D3D12API::Get()->GetDevicePtr()->CreateRenderTargetView(
			rt.pResource.Get(), &desc, rt.descriptorHeapHandle.cpuHandle
		);
	}

	void RenderTargetManager::CreateDepthStencilView(RenderTarget& rt)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;

		D3D12API::Get()->GetDevicePtr()->CreateDepthStencilView(
			rt.pResource.Get(), &desc, rt.descriptorHeapHandle.cpuHandle
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
