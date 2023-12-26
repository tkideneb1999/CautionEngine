#include "D3D12DescriptorHeap.h"

#include <sstream>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering::D3D12
{
	DescriptorHeap::DescriptorHeap(
		ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool isShaderVisible
	)
	{
		if (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			isShaderVisible = false;
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = isShaderVisible? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

#if _DEBUG
		std::stringstream report;
		report << "Failed to Create Descriptor Heap for type: " << type;
#endif
		
		ThrowIfFailed(
			pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_DescriptorHeap)),
#if _DEBUG
			report.str().c_str()
#else
			"Failed to create Descriptor Heap"
#endif
		);
		m_incrementSize = pDevice->GetDescriptorHandleIncrementSize(type);
		m_cpuStartHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		m_gpuStartHandle = m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}

	DescriptorHeapHandle DescriptorHeap::GetHandle(SIZE_T index)
	{
		DescriptorHeapHandle handle = {};
		handle.cpuHandle.ptr = m_cpuStartHandle.ptr + index * m_incrementSize;
		handle.gpuHandle.ptr = m_gpuStartHandle.ptr + index * m_incrementSize;
		return handle;
	}
}