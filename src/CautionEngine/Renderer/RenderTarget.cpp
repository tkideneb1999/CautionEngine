#include "stdafx.h"
#include "RenderTarget.h"

namespace CautionEngine::Rendering
{
	RenderTarget::RenderTarget(RenderFormat format, unsigned int width, unsigned int height, unsigned int id, 
		int mipLevels, float* clearValue)
		: descriptorHeapHandle()
		, pResource()
		, m_renderFormat(format)
		, m_width(width), m_height(height)
		, m_mipLevels(mipLevels)
		, m_id(id)
		, m_isDepthRT(format == RENDER_FORMAT_D32_FLOAT)
	{
		if (m_isDepthRT)
		{
			m_clearValue[0] = *clearValue;
		}
		else
		{
			memcpy(&m_clearValue[0], clearValue, 4 * sizeof(float));
		}
	}
	RenderTarget::RenderTarget(D3D12::DescriptorHeapHandle& handle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
		: descriptorHeapHandle(handle)
		, pResource(resource)
		, m_renderFormat()
		, m_width(0), m_height(0)
		, m_mipLevels(0)
		, m_id(0)
		, m_isDepthRT(false)
		, m_clearValue()
	{
	}
}
