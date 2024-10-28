#include "stdafx.h"
#include "ConstantBuffer.h"

#include "D3D12API.h"
#include "DescriptorManager.h"

namespace CautionEngine::Rendering
{
	bool ConstantBufferLayout::AppendData(unsigned int dataSize, std::string& name, CBufferVariableType type)
	{
		if (m_nameMapping.find(name) != m_nameMapping.end())
		{
			return false;
		}
		m_nameMapping.insert(std::pair<std::string, size_t>(name, m_layout.size()));
		m_layout.emplace_back(dataSize, m_size + dataSize, type);
		m_size += dataSize;
		return true;
	}

	inline bool ConstantBuffer::SetData(size_t index, void* pData, size_t size, CBufferVariableType type)
	{
		ConstantBufferLayout::ConstantBufferElement& layoutElement = m_bufferLayout.m_layout[index];
		if (size != layoutElement.m_size || layoutElement.m_type != type)
		{
			return false;
		}
		void* pDataLocation = m_pBufferMemory + layoutElement.m_byteOffset;
		std::memcpy(pDataLocation, pData, size);
		return true;
	}

	inline bool ConstantBuffer::SetDataFromName(std::string& name, void* pData, size_t size, CBufferVariableType type)
	{
		int index = GetIndexFromName(name);
		if (index < 0)
		{
			return false;
		}
		return SetData(index, pData, size, type);
	}

	void ConstantBuffer::CreateUploadBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& pUploadBuffer)
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		resourceDesc.Width = GetAlignedSizeInBytes();
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.CreationNodeMask = 0;
		heapProps.VisibleNodeMask = 0;

		const ComPtr<ID3D12Device8>& pDevice = D3D12API::Get()->GetDevicePtr();
		THROW_IF_FAILED(
			pDevice->CreateCommittedResource(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pUploadBuffer)
			),
			"Can not create committed Resource for Constant Buffer"
		);
		pUploadBuffer->SetName(L"CBuffer Resource Heap");
	}

	ConstantBuffer::ConstantBuffer(ConstantBufferLayout& layout, unsigned int numBackBuffers)
	{
		m_bufferLayout = layout;
		m_pBufferMemory = (byte*)malloc(layout.m_size);
		if (m_pBufferMemory != nullptr)
		{
			ZeroMemory(m_pBufferMemory, layout.m_size);
		}
		m_bufferResources.resize(numBackBuffers);
	}

	ConstantBuffer::~ConstantBuffer()
	{
		free(m_pBufferMemory);
		m_pBufferMemory = nullptr;
	}

	void ConstantBuffer::Init(DescriptorManager* pDescriptorManager)
	{
		using namespace Microsoft::WRL;
		const ComPtr<ID3D12Device8>& pDevice = D3D12API::Get()->GetDevicePtr();

		for (size_t i = 0; i < m_bufferResources.size(); ++i)
		{
			ComPtr<ID3D12Resource>& pResource = m_bufferResources[i];
			CreateUploadBuffer(pResource);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pResource->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = CalcNearestMultiple(m_bufferLayout.m_size, 256);
			D3D12::DescriptorHeapHandle handle = pDescriptorManager->AllocateCbvSrvUav();
			pDevice->CreateConstantBufferView(&cbvDesc, handle.cpuHandle);
			UpdateGPUMemory(i);
		}
	}

	void ConstantBuffer::UpdateGPUMemory(int backBufferIndex)
	{
		D3D12_RANGE range = {};
		range.Begin = 0;
		range.End = 0;
		void* pData = nullptr;
		THROW_IF_FAILED(
			m_bufferResources[backBufferIndex]->Map(0, &range, &pData),
			"Can not map constant buffer"
		);
		memcpy(pData, m_pBufferMemory, GetSizeInBytes());
	}

	int ConstantBuffer::GetIndexFromName(std::string& name)
	{
		auto indexElement = m_bufferLayout.m_nameMapping.find(name);
		if (indexElement == m_bufferLayout.m_nameMapping.end())
		{
			// Invalid Name
			return -1;
		}
		return indexElement->second;
	}
}