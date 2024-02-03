#include "D3D12API.h"

#include <exception>
#include <iostream>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	D3D12API::D3D12API()
	{
	}

	void D3D12API::Init()
	{
		if (m_initialized)
			return;
#if _DEBUG
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)), "Couldn't get Debug Interface");
			debugController->EnableDebugLayer();

			ComPtr<ID3D12DeviceRemovedExtendedDataSettings1> dredSettings;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&dredSettings)), "Couldn't get Debug Interface");
			dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
			dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		}
#endif

		ThrowIfFailed(
			CreateDXGIFactory2( // TODO: benedikt -> you might want to initialize the DXGI factory outside of this project?
				// https://gamedev.stackexchange.com/questions/128197/why-do-i-get-this-error-about-dllmain-when-using-d3d-from-within-a-dll
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
#if _DEBUG
		std::cout << "Graphics Device initialized!" << std::endl;
#endif
		m_initialized = true;
	}

	void D3D12API::GatherDREDOUTput()
	{
		ComPtr<ID3D12DeviceRemovedExtendedData1> dredData;
		ThrowIfFailed(m_device->QueryInterface(dredData.GetAddressOf()), "Query Interface Failed");

		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT1 dredOutput{};
		D3D12_DRED_PAGE_FAULT_OUTPUT1 dredPageFaultOutput{};
		dredData->GetAutoBreadcrumbsOutput1(&dredOutput);
		dredData->GetPageFaultAllocationOutput1(&dredPageFaultOutput);
		DebugBreak();
	}

	void D3D12API::GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref)
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