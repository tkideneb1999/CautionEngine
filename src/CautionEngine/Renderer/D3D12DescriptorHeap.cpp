#include "stdafx.h"

#include "D3D12DescriptorHeap.h"

#include <sstream>
#include <iostream>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering::D3D12
{
	DescriptorHeap::DescriptorHeap(
		ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool isShaderVisible
	) : m_isShaderVisible(isShaderVisible)
	{
		if (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			isShaderVisible = false;
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = isShaderVisible? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

#if _DEBUG
		std::stringstream report;
		report << "Failed to Create Descriptor Heap for type: " << type << std::endl;
#endif
		
		THROW_IF_FAILED(
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
		m_maxDescriptors = numDescriptors;
	}

	DescriptorHeapHandle DescriptorHeap::GetHandle(SIZE_T index)
	{
		DescriptorHeapHandle handle = {};
		handle.cpuHandle.ptr = m_cpuStartHandle.ptr + index * m_incrementSize;
		handle.gpuHandle.ptr = m_gpuStartHandle.ptr + index * m_incrementSize;
		return handle;
	}

	DescriptorHeapHandle DescriptorHeap::Allocate()
	{
		if (m_allocatedDescriptors >= m_maxDescriptors)
			throw std::exception("Maximum Number of Descriptors Allocated");

		DescriptorHeapHandle allocatedHandle = GetHandle(m_allocatedDescriptors);
		m_allocatedDescriptors++;
#if _DEBUG
		std::cout << "Allocated Descriptor: " << m_allocatedDescriptors << "/" << m_maxDescriptors << std::endl;
#endif
		return allocatedHandle;
	}
}