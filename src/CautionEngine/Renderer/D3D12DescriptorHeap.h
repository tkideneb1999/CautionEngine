#pragma once
#include <d3d12.h>

#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering::D3D12
{
	struct DescriptorHeapHandle 
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	class DescriptorHeap
	{
	private:
		ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap = {};
		UINT m_incrementSize;
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStartHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE m_gpuStartHandle;

	public:
		DescriptorHeap()
			:m_DescriptorHeap(), m_incrementSize(0), m_cpuStartHandle(),m_gpuStartHandle()
		{};
		DescriptorHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool isShaderVisible);
		DescriptorHeapHandle GetHandle(SIZE_T index);
	};
}