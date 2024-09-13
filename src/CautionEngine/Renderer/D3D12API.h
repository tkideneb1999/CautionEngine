#pragma once
#include <CautionDefinitions.h>
#include "D3D12DescriptorHeap.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <exception>

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	class CAUTION_API D3D12API
	{
	public:
		D3D12API();
		~D3D12API();

		// Disable Copying
		D3D12API(const D3D12API& original) = delete;
		D3D12API& operator=(const D3D12API& original) = delete;

		static D3D12API* const Get();

		const ComPtr<ID3D12Device8> GetDevicePtr()
		{ 
			if (m_initialized)
				return m_device;
			throw std::exception("D3D12 not initialized!");
		}
		const ComPtr<IDXGIFactory7> GetFactoryPtr()
		{ 
			if(m_initialized)
				return m_factory;
			throw std::exception("D3D12 not initialized!");
		}

		// ! \brief Initializes the API
		void Init();
		// ! \brief Initiates the shutdown process and releases memory
		void Shutdown();

		void GatherDREDOUTput();

		bool IsInitialized() const { return m_initialized; }

	private:
		bool m_initialized = false;
		ComPtr<ID3D12Device8> m_device;
		ComPtr<IDXGIFactory7> m_factory;
		ComPtr<IDXGIAdapter4> m_adapter;

		void GetAdapter(IDXGIAdapter4** ppAdapter, DXGI_GPU_PREFERENCE pref);
	};
}