#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "D3D12DescriptorHeap.h"

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering 
{
	class D3D12API
	{
	private:
		ComPtr<ID3D12Device8> m_device;
		ComPtr<IDXGIFactory7> m_factory;
		ComPtr<IDXGIAdapter4> m_adapter;

		D3D12::DescriptorHeap m_cbv_srv_uav_descHeap;
		D3D12::DescriptorHeap m_rtv_descHeap;
		D3D12::DescriptorHeap m_dsv_descHeap;
		D3D12::DescriptorHeap m_sampler_descHeap;

	public:
		D3D12API();

		// Disable Copying
		D3D12API(const D3D12API& original) = delete;
		D3D12API& operator=(const D3D12API& original) = delete;

	private:
		void GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref);
	};
}