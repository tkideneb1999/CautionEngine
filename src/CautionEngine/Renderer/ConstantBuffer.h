#pragma once

#include <CautionDefinitions.h>

#include <d3d12.h>
#include <vector>

#include <glm/common.hpp>

#include "ShaderData.h"
#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	class DescriptorManager;
	// Forward Declare for BufferLayout
	class ConstantBuffer;

	class ConstantBufferLayout
	{
		friend class ConstantBuffer;
	private:
		struct ConstantBufferElement
		{
			unsigned int m_size;
			unsigned int m_byteOffset;
			CBufferVariableType m_type;
			std::string name;

			ConstantBufferElement(unsigned int size, unsigned int byteOffset, CBufferVariableType type)
				:m_size(size), m_byteOffset(byteOffset), m_type(type)
			{}
		};

		std::vector<ConstantBufferElement> m_layout;
		std::unordered_map<std::string, size_t> m_nameMapping;
		size_t m_size;

		inline bool AppendData(unsigned int dataSize, std::string& name, CBufferVariableType type);

	public:
		bool AppendFloat(std::string& name) { return AppendData(sizeof(float), name, CBUFFER_VAR_FLOAT); }
		bool AppendInt(std::string& name) { return AppendData(sizeof(int), name, CBUFFER_VAR_INT); }
		bool AppendUInt(std::string& name) { return AppendData(sizeof(unsigned int), name, CBUFFER_VAR_UINT); }
		bool AppendVec2(std::string& name) { return AppendData(sizeof(glm::vec2), name, CBUFFER_VAR_FLOAT2); }
		bool AppendVec3(std::string& name) { return AppendData(sizeof(glm::vec3), name, CBUFFER_VAR_FLOAT3); }
		bool AppendVec4(std::string& name) { return AppendData(sizeof(glm::vec4), name, CBUFFER_VAR_FLOAT4); }
		bool AppendMat4x4(std::string& name) { return AppendData(sizeof(glm::mat4x4), name, CBUFFER_VAR_MAT44); }
	};

	class ConstantBuffer
	{
	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_bufferResources;
		ConstantBufferLayout m_bufferLayout;
		byte* m_pBufferMemory;

		inline bool SetData(size_t index, void* pData, size_t size, CBufferVariableType type);
		inline bool SetDataFromName(std::string& name, void* pData, size_t size, CBufferVariableType type);

		void CreateUploadBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& pUploadBuffer);

	public:
		ConstantBuffer(ConstantBufferLayout& layout, unsigned int numBackBuffers);
		ConstantBuffer(ConstantBuffer& other) = delete;
		~ConstantBuffer();

		void Init(DescriptorManager* pDescriptorManager);
		void UpdateGPUMemory(int backBufferIndex);

		bool SetFloat(size_t index, float* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(float), CBUFFER_VAR_FLOAT); }
		bool SetInt(size_t index, int* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(int), CBUFFER_VAR_INT); }
		bool SetUInt(size_t index, unsigned int* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(unsigned int), CBUFFER_VAR_UINT); }
		bool SetVec2(size_t index, glm::vec2* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(glm::vec2), CBUFFER_VAR_FLOAT2); }
		bool SetVec3(size_t index, glm::vec3* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(glm::vec3), CBUFFER_VAR_FLOAT3); }
		bool SetVec4(size_t index, glm::vec4* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(glm::vec4), CBUFFER_VAR_FLOAT4); }
		bool SetMat4x4(size_t index, glm::mat4x4* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(glm::mat4x4), CBUFFER_VAR_MAT44); }

		bool SetFloat(std::string& name, float* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(float), CBUFFER_VAR_FLOAT); }
		bool SetInt(std::string& name, int* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(int), CBUFFER_VAR_INT); }
		bool SetUInt(std::string& name, unsigned int* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(unsigned int), CBUFFER_VAR_UINT); }
		bool SetVec2(std::string& name, glm::vec2* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(glm::vec2), CBUFFER_VAR_FLOAT2); }
		bool SetVec3(std::string& name, glm::vec3* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(glm::vec3), CBUFFER_VAR_FLOAT3); }
		bool SetVec4(std::string& name, glm::vec4* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(glm::vec4), CBUFFER_VAR_FLOAT4); }
		bool SetMat4x4(std::string& name, glm::mat4x4* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(glm::mat4x4), CBUFFER_VAR_MAT44); }

		size_t GetSizeInBytes() const { return m_bufferLayout.m_size; }
		size_t GetAlignedSizeInBytes() const { return CalcNearestMultiple(m_bufferLayout.m_size, 64); }
		size_t GetElementCount() const { return m_bufferLayout.m_layout.size(); }
		int GetIndexFromName(std::string& name);
		CBufferVariableType GetType(size_t index) const { return m_bufferLayout.m_layout[index].m_type; }
	};
}