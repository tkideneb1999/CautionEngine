#pragma once
#include "../CautionDefinitions.h"

#include "D3D12DescriptorHeap.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	class CAUTION_API D3D12API
	{
	private:
		ComPtr<ID3D12Device8> m_device;
		ComPtr<IDXGIFactory7> m_factory;
		ComPtr<IDXGIAdapter4> m_adapter;

	public:
		D3D12API();

		// Disable Copying
		D3D12API(const D3D12API& original) = delete;
		D3D12API& operator=(const D3D12API& original) = delete;

		const ComPtr<ID3D12Device8> GetDevicePtr() { return m_device; }
		const ComPtr<IDXGIFactory7> GetFactoryPtr() { return m_factory; }

		void GatherDREDOUTput();

	private:
		void GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref);
	};
}