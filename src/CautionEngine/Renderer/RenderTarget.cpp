#include "stdafx.h"
#include "RenderTarget.h"

namespace CautionEngine::Rendering
{
	RenderTarget::RenderTarget()
		:descriptorHeapHandle(), resourceView()
	{

	}

	RenderTarget::RenderTarget(D3D12::DescriptorHeapHandle handle, Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetResource)
	{
		descriptorHeapHandle = handle;
		resourceView = renderTargetResource;
	}
}
