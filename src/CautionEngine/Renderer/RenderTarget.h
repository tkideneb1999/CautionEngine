#pragma once

#include <CautionEngineDefinitions.h>
#include "Formats.h"
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

		RenderTarget(
			RenderFormat format, unsigned int width, unsigned int height, unsigned int id, 
			int mipLevels, float clearValue[]
		);

		RenderTarget(D3D12::DescriptorHeapHandle& handle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);

		const unsigned int GetWidth() const { return m_width; }
		const unsigned int GetHeight() const { return m_height; }
		const unsigned int GetId() const { return m_id; }
		const bool IsDepthRT() const { return m_isDepthRT; }
		const RenderFormat GetRenderFormat() const { return m_renderFormat; }

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
		unsigned int m_id = 0;
		RenderFormat m_renderFormat;
		bool m_isDepthRT;
		unsigned int m_mipLevels;
		float m_clearValue[4];
	};
}