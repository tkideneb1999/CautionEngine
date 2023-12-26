#include "D3D12API.h"

#include <exception>
#include <iostream>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	D3D12API::D3D12API()
	{
		ThrowIfFailed(
			CreateDXGIFactory2(
#if _DEBUG
				DXGI_CREATE_FACTORY_DEBUG,
#endif
				IID_PPV_ARGS(&m_factory)
			),
			"Creating Factory failed"
		);

		//TODO: Make GPU pref accessible
		GetAdapter(&m_adapter, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);
		ThrowIfFailed(
			D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)),
			"Device Creation Failed"
			);

		m_cbv_srv_uav_descHeap = D3D12::DescriptorHeap(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 16, true);
		m_dsv_descHeap = D3D12::DescriptorHeap(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 16, true);
		m_rtv_descHeap = D3D12::DescriptorHeap(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 16, true);
		m_sampler_descHeap = D3D12::DescriptorHeap(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 16, true);
	}

	void D3D12API::GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref
	)
	{
		*ppAdapter = nullptr;
		
		ThrowIfFailed(
			m_factory->EnumAdapterByGpuPreference(0, pref, IID_PPV_ARGS(ppAdapter)), 
			"Couldn't find GPU Adapter"
		);

#if _DEBUG
		//TODO: Proper logging
		std::cout << "Found Adapter:" << std::endl;
		DXGI_ADAPTER_DESC3 adapterDesc;
		(*ppAdapter)->GetDesc3(&adapterDesc);
		
		std::wcout << "  Description: " << adapterDesc.Description << std::endl;
		std::wcout << "  Dedicated System Memory: " << adapterDesc.DedicatedSystemMemory / 1000000 << "MB" << std::endl;
		std::wcout << "  Shared System Memory: " << adapterDesc.SharedSystemMemory / 1000000 << "MB" << std::endl;
		std::wcout << "  Dedicated Video Memory: " << adapterDesc.DedicatedVideoMemory / 1000000 << "MB" << std::endl;
		std::wcout << "  Vendor ID: " << adapterDesc.VendorId << std::endl;
#endif
	}
}