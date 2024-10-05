#include "stdafx.h"
#include "Mesh.h"

#include "D3D12API.h"
#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	Mesh::Mesh()
		: m_vertices()
		, m_indices()
		, m_pVertexBufferResource()
		, m_pIndexBufferResource()
	{

	}

	Mesh::~Mesh()
	{
		Unload();
	}

	void Mesh::ScheduleUpload(ID3D12GraphicsCommandList* cmdList)
	{
		// TODO: (Put Bigger reusable Upload Heap to Renderer)
		using namespace Microsoft::WRL;

		m_vertexBufferView.StrideInBytes = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes = m_vertexBufferView.StrideInBytes * m_vertices.size();
		UINT vertexBufferSize = m_vertexBufferView.SizeInBytes;

		m_indexBufferView.SizeInBytes = sizeof(UINT32) * m_indices.size();
		m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		UINT indexBufferSize = m_indexBufferView.SizeInBytes;

		UINT uploadBufferSize = vertexBufferSize + indexBufferSize;

		CreateUploadBuffer(vertexBufferSize, m_pVertexUploadHeap);
		m_pVertexUploadHeap->SetName(L"Mesh Upload Heap");
		CreateUploadBuffer(indexBufferSize, m_pIndexUploadHeap);
		m_pIndexUploadHeap->SetName(L"Index Upload Heap");

		CreateMeshBuffer(vertexBufferSize, m_pVertexBufferResource);
		m_pVertexBufferResource->SetName(L"Mesh Vertex Buffer");
		CreateMeshBuffer(indexBufferSize, m_pIndexBufferResource);
		m_pIndexBufferResource->SetName(L"Mesh Index Buffer");

		// Copy from Vertex Data from Mesh to UploadBuffer
		void* pVertexDataUpload;
		D3D12_RANGE vertexBufferRange{};
		vertexBufferRange.Begin = 0;
		vertexBufferRange.End = vertexBufferSize;
		m_pVertexUploadHeap->Map(0, &vertexBufferRange, &pVertexDataUpload);
		memcpy(pVertexDataUpload, m_vertices.data(), vertexBufferSize);
		m_pVertexUploadHeap->Unmap(0, &vertexBufferRange);

		// Copy Index Data from Mesh to Uplaod Buffer
		void* pIndexDataUpload;
		D3D12_RANGE indexBufferRange{};
		indexBufferRange.Begin = 0;
		indexBufferRange.End = indexBufferSize;
		THROW_IF_FAILED(
			m_pIndexUploadHeap->Map(0, &indexBufferRange, &pIndexDataUpload),
			"Could not map Index Buffer Range"
		);
		memcpy(pIndexDataUpload, m_indices.data(), indexBufferSize);
		m_pIndexUploadHeap->Unmap(0, &indexBufferRange);

		// Copy from Upload Buffer to Vertex Buffer
		cmdList->CopyBufferRegion(
			m_pVertexBufferResource.Get(), 0, m_pVertexUploadHeap.Get(), 0, vertexBufferSize
		);
		// Copy from Upload Buffer to Index Buffer
		cmdList->CopyBufferRegion(
			m_pIndexBufferResource.Get(), 0, m_pIndexUploadHeap.Get(), 0, indexBufferSize
		);
		D3D12_RESOURCE_BARRIER bufferBarriers[2] = {
			CreateResourceBarrierTransition(
			m_pVertexBufferResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
			),
			CreateResourceBarrierTransition(
			m_pIndexBufferResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER
			),
		};
		cmdList->ResourceBarrier(2, &bufferBarriers[0]);

		m_vertexBufferView.BufferLocation = m_pVertexBufferResource->GetGPUVirtualAddress();
		m_indexBufferView.BufferLocation = m_pIndexBufferResource->GetGPUVirtualAddress();

		m_uploaded = true;
	}

	void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
	{
		if (!m_uploaded)
		{
			return;
		}
		cmdList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		cmdList->IASetIndexBuffer(&m_indexBufferView);
		cmdList->DrawIndexedInstanced(m_indicesAmount, 1, 0, 0, 0);
	}

	void Mesh::Unload()
	{
		m_uploaded = false;

		m_vertexBufferView.BufferLocation = 0;
		m_indexBufferView.BufferLocation = 0;

		if (m_pVertexBufferResource)
		{
			m_pVertexBufferResource->Release();
		}

		if (m_pIndexBufferResource)
		{
			m_pIndexBufferResource->Release();
		}
	}

	void Mesh::SetVertices(Vertex* pVertices, size_t length)
	{
		m_vertices.clear();
		m_vertices.reserve(length);
		for (size_t i = 0; i < length; ++i)
		{
			m_vertices.push_back(pVertices[i]);
		}
	}

	void Mesh::SetIndices(int* pIndices, size_t length)
	{
		m_indices.clear();
		m_indices.reserve(length);
		for (size_t i = 0; i < length; ++i)
		{
			m_indices.push_back(pIndices[i]);
		}
		m_indicesAmount = length;
	}

	void Mesh::CreateUploadBuffer(UINT& size, Microsoft::WRL::ComPtr<ID3D12Resource>& pUploadHeap)
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		resourceDesc.Width = size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		uploadHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		uploadHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		uploadHeapProperties.CreationNodeMask = 0;
		uploadHeapProperties.VisibleNodeMask = 0;

		const ComPtr<ID3D12Device8>& pDevice = D3D12API::Get()->GetDevicePtr();
		THROW_IF_FAILED(
			pDevice->CreateCommittedResource(
				&uploadHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pUploadHeap)
			),
			"Upload Heap could not be created"
		);
	}

	void Mesh::CreateMeshBuffer(UINT& size, Microsoft::WRL::ComPtr<ID3D12Resource>& pBuffer)
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		resourceDesc.Width = size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		D3D12_HEAP_PROPERTIES defaultHeapProperties = {};
		defaultHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		defaultHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		defaultHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		defaultHeapProperties.CreationNodeMask = 0;
		defaultHeapProperties.VisibleNodeMask = 0;

		const ComPtr<ID3D12Device8>& pDevice = D3D12API::Get()->GetDevicePtr();
		THROW_IF_FAILED(
			pDevice->CreateCommittedResource(
				&defaultHeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&pBuffer)
			),
			"Default Heap could not be created"
		);
	}
}