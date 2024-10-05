#include "stdafx.h"

#include "D3D12API.h"

#include <exception>
#include <iostream>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	extern D3D12API* g_pD3D12API;

	extern CAUTION_CLASS void InitD3D12API()
	{
		g_pD3D12API = new D3D12API();
		g_pD3D12API->Init();
	}

	extern CAUTION_CLASS void ShutdownD3D12API()
	{
		g_pD3D12API->Shutdown();
		delete g_pD3D12API;
	}

	D3D12API::D3D12API()
	{
	}

	D3D12API::~D3D12API()
	{
		Shutdown();
	}

	D3D12API* const D3D12API::Get()
	{
		return g_pD3D12API;
	}

	void D3D12API::Init()
	{
		if (m_initialized)
			return;
#if _DEBUG
		{
			ComPtr<ID3D12Debug1> debugController;
			THROW_IF_FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)), "Couldn't get Debug Interface");
			debugController->EnableDebugLayer();
			debugController->SetEnableGPUBasedValidation(true);
		}
#endif

		THROW_IF_FAILED(
			CreateDXGIFactory2(
#if _DEBUG
				DXGI_CREATE_FACTORY_DEBUG,
#else
				0,
#endif
				IID_PPV_ARGS(&m_factory)
			),
			"Creating Factory failed"
		);

		//TODO: Make GPU pref accessible
		GetAdapter(&m_adapter, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);
		THROW_IF_FAILED(
			D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)),
			"Device Creation Failed"
		);
#if _DEBUG
		std::cout << "Graphics Device initialized!" << std::endl;
#endif
		m_initialized = true;
	}

	void D3D12API::Shutdown()
	{
		if (!m_initialized)
		{
			return;
		}

#if _DEBUG
		ComPtr<ID3D12DebugDevice> pDebugDevice;
		m_device->QueryInterface(IID_PPV_ARGS(&pDebugDevice));
		if (pDebugDevice)
		{
			pDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
		}
#endif

		m_factory.Reset();
		m_factory = nullptr;
		m_device.Reset();
		m_device = nullptr;
		m_initialized = false;
	}

	void D3D12API::GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref)
	{
		*ppAdapter = nullptr;
		
		THROW_IF_FAILED(
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