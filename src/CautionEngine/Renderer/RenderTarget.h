#pragma once

#include <CautionEngineDefinitions.h>
#include "D3D12DescriptorHeap.h"

namespace CautionEngine::Rendering
{
	// forward declaration from RenderTargetManager.h
	class RenderTargetManager;

	class CAUTION_ENGINE_CLASS RenderTarget
	{
		friend class RenderTargetManager;
	public:
		D3D12::DescriptorHeapHandle descriptorHeapHandle;
		Microsoft::WRL::ComPtr<ID3D12Resource> pResource; // TODO: Make this private

		const Microsoft::WRL::ComPtr<ID3D12Resource>& GetPtr() { return pResource; }

		RenderTarget();

		// TODO: Remove this
		RenderTarget(D3D12::DescriptorHeapHandle handle, Microsoft::WRL::ComPtr<ID3D12Resource> renderTargetResource);

		const unsigned int GetWidth() { return m_width; }
		const unsigned int GetHeight() { return m_height; }
		const unsigned int GetId() { return m_id; }

		void Release() 
		{ 
			if (pResource) 
			{ 
				pResource.Reset();
			} 
		}

	private:
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		unsigned int m_id;
	};
}