#pragma once

#include <CautionDefinitions.h>
#include "D3D12DescriptorHeap.h"

namespace CautionEngine::Rendering
{
	class CAUTION_API RenderTarget
	{
	public:
		D3D12::DescriptorHeapHandle descriptorHeapHandle;
		Microsoft::WRL::ComPtr<ID3D12Resource> resourceView;

		const Microsoft::WRL::ComPtr<ID3D12Resource> GetPtr() { return resourceView; }

		RenderTarget();
		RenderTarget(D3D12::DescriptorHeapHandle handle, Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetResource);
	};
}