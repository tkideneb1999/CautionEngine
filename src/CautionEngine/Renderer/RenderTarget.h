#pragma once

#include <CautionDefinitions.h>
#include "D3D12DescriptorHeap.h"

namespace CautionEngine::Rendering
{
	// forward declaration from RenderTargetManager.h
	class RenderTargetManager;

	class CAUTION_CLASS RenderTarget
	{
		friend class RenderTargetManager;
	public:
		D3D12::DescriptorHeapHandle descriptorHeapHandle;
		Microsoft::WRL::ComPtr<ID3D12Resource> resourceView;

		const Microsoft::WRL::ComPtr<ID3D12Resource> GetPtr() { return resourceView; }

		RenderTarget();
		RenderTarget(D3D12::DescriptorHeapHandle handle, Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetResource);

		
		const unsigned int GetWidth();
		const unsigned int GetHeight();

		void Release();
	};
}