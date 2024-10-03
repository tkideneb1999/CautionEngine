#pragma once
#include <CautionDefinitions.h>

#include <d3d12.h>
#include <glm/glm.hpp>

namespace CautionEngine::Rendering
{
	class CAUTION_CLASS Vertex
	{
	public:
		glm::vec4 position;
		glm::vec4 color;
	};

	class CAUTION_CLASS Mesh
	{
	public:
		Mesh();
		~Mesh();

		void ScheduleUpload(ID3D12GraphicsCommandList* cmdList); // TODO: Refactor this
		void Draw(ID3D12GraphicsCommandList* cmdList);
		void Unload();
		bool IsUploaded() const { return m_uploaded; }

		void SetVertices(Vertex* pVertices, size_t length);
		void SetIndices(int* pIndices, size_t length);

	private:
		bool m_uploaded = false;
		std::vector<Vertex> m_vertices;
		std::vector<UINT32> m_indices;
		UINT m_indicesAmount = 0;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBufferResource;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView{};
		
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pIndexBufferResource;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView{};

		Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexUploadHeap; // Exists only until Resources have been uploaded
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pIndexUploadHeap;

		void CreateUploadBuffer(UINT& size, Microsoft::WRL::ComPtr<ID3D12Resource>& pUploadHeap); // TODO: Move this out of the class
		void CreateMeshBuffer(UINT& size, Microsoft::WRL::ComPtr<ID3D12Resource>& pBuffer);
	};
}
