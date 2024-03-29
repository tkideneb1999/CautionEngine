#include "stdafx.h"
#include "RenderTargetManager.h"

#include "Renderer.h"

namespace CautionEngine::Rendering 
{
	RenderTargetManager::RenderTargetManager(DescriptorManager* pDescriptorManager)
	{
		m_pDescriptorManager = pDescriptorManager;
	}

	RenderTarget* RenderTargetManager::CreateRenderTarget(unsigned int width, unsigned int height, RenderFormat format)
	{
		RenderTarget rt = RenderTarget();
		if (format == RENDER_FORMAT_D32_FLOAT)
		{
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateDSV();
		}
		else
		{
			rt.descriptorHeapHandle = m_pDescriptorManager->AllocateRTV();
			D3D12_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = (DXGI_FORMAT)format;

			Renderer::s_api.GetDevicePtr()->CreateRenderTargetView(
				rt.resourceView.Get(), &desc, rt.descriptorHeapHandle.cpuHandle
			);
		}
		
		return nullptr;
	}
}
