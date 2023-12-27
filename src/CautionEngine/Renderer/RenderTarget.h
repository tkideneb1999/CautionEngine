#pragma once

#include "D3D12DescriptorHeap.h"

namespace CautionEngine::Rendering
{
	class RenderTarget
	{
	public:
		D3D12::DescriptorHeapHandle descriptorHeapHandle;
		Microsoft::WRL::ComPtr<ID3D12Resource> resourceView;

		const Microsoft::WRL::ComPtr<ID3D12Resource> GetPtr() { return resourceView; }

		RenderTarget() = delete;
		RenderTarget(D3D12::DescriptorHeapHandle handle, Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetResource);
	};
}