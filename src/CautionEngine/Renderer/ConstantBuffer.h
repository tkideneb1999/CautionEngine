#pragma once

#include <CautionEngineDefinitions.h>

#include <d3d12.h>
#include <vector>

#include <glm/common.hpp>

#include "ShaderData.h"
#include "D3D12Helpers.h"

namespace CautionEngine::Rendering
{
	class DescriptorManager;
	class D3D12ShaderCompiler;
	// Forward Declare for BufferLayout
	class ConstantBuffer;

	class ConstantBufferLayout
	{
		friend class ConstantBuffer;
		friend class D3D12ShaderCompiler;
	private:
		struct ConstantBufferElement
		{
			unsigned int m_size;
			unsigned int m_byteOffset;

			// Type Description
			ShaderVariableTypes m_type;
			unsigned int m_columns; // Number of dimensions for vectors, columns for matrices
			unsigned int m_rows; // 1 for vectors, rows for matrices

			std::string name;

			ConstantBufferElement(
				unsigned int size, unsigned int byteOffset, ShaderVariableTypes type, unsigned int columns, unsigned int rows
			)
				:m_size(size), m_byteOffset(byteOffset), m_type(type), m_columns(columns), m_rows(rows)
			{}
		};

		std::vector<ConstantBufferElement> m_layout;
		std::unordered_map<std::string, size_t> m_nameMapping;
		size_t m_size = 0;
		std::string m_name;

		bool AppendData(
			unsigned int dataSize, std::string& name, ShaderVariableTypes type, unsigned int columns, unsigned int rows
		);

	public:
		ConstantBufferLayout(std::string& name)
			:m_name(name)
		{

		}

		std::string const & GetName() const { return m_name; }
		bool IsEqual(ConstantBufferLayout const& other);
	};

	class ConstantBuffer
	{
	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_bufferResources;
		ConstantBufferLayout m_bufferLayout;
		byte* m_pBufferMemory;

		inline bool SetData(size_t index, void* pData, size_t size, ShaderVariableTypes type, unsigned int columns, unsigned int rows);
		inline bool SetDataFromName(std::string& name, void* pData, size_t size, ShaderVariableTypes type, unsigned int columns, unsigned int rows);

		void CreateUploadBuffer(Microsoft::WRL::ComPtr<ID3D12Resource>& pUploadBuffer);

	public:
		ConstantBuffer(ConstantBufferLayout& layout, unsigned int numBackBuffers);
		ConstantBuffer(ConstantBuffer& other) = delete;
		~ConstantBuffer();

		void Init(DescriptorManager* pDescriptorManager);
		void UpdateGPUMemory(int backBufferIndex);

#define DECLARE_SET(name, type, shaderVarType, columns, rows) \
		bool name(size_t index, type* pValue) { return SetData(index, static_cast<void*>(pValue), sizeof(type), shaderVarType, columns, rows); } \
		bool name(std::string& name, type* pValue) { return SetDataFromName(name, static_cast<void*>(pValue), sizeof(type), shaderVarType, columns, rows); }

		DECLARE_SET(SetUInt, unsigned int, SHADER_VAR_TYPE_UINT, 1, 1)
		DECLARE_SET(SetUInt2, glm::uvec2, SHADER_VAR_TYPE_UINT, 2, 1)
		DECLARE_SET(SetUInt3, glm::uvec3, SHADER_VAR_TYPE_UINT, 3, 1)
		DECLARE_SET(SetUInt4, glm::uvec4, SHADER_VAR_TYPE_UINT, 4, 1)

		DECLARE_SET(SetInt, int, SHADER_VAR_TYPE_INT, 1, 1)
		DECLARE_SET(SetInt2, glm::ivec2, SHADER_VAR_TYPE_INT, 2, 1)
		DECLARE_SET(SetInt3, glm::ivec3, SHADER_VAR_TYPE_INT, 3, 1)
		DECLARE_SET(SetInt4, glm::ivec4, SHADER_VAR_TYPE_INT, 4, 1)

		DECLARE_SET(SetFloat, float, SHADER_VAR_TYPE_FLOAT, 1, 1)
		DECLARE_SET(SetFloat2, glm::vec2, SHADER_VAR_TYPE_FLOAT, 2, 1)
		DECLARE_SET(SetFloat3, glm::vec3, SHADER_VAR_TYPE_FLOAT, 3, 1)
		DECLARE_SET(SetFloat4, glm::vec4, SHADER_VAR_TYPE_FLOAT, 4, 1)

		DECLARE_SET(SetFloat2x2, glm::mat2x2, SHADER_VAR_TYPE_FLOAT, 2, 2)
		DECLARE_SET(SetFloat2x3, glm::mat2x3, SHADER_VAR_TYPE_FLOAT, 2, 3)
		DECLARE_SET(SetFloat2x4, glm::mat2x4, SHADER_VAR_TYPE_FLOAT, 2, 4)

		DECLARE_SET(SetFloat3x2, glm::mat3x2, SHADER_VAR_TYPE_FLOAT, 3, 2)
		DECLARE_SET(SetFloat3x3, glm::mat3x3, SHADER_VAR_TYPE_FLOAT, 3, 3)
		DECLARE_SET(SetFloat3x4, glm::mat3x4, SHADER_VAR_TYPE_FLOAT, 3, 4)

		DECLARE_SET(SetFloat4x2, glm::mat4x2, SHADER_VAR_TYPE_FLOAT, 4, 2)
		DECLARE_SET(SetFloat4x3, glm::mat4x3, SHADER_VAR_TYPE_FLOAT, 4, 3)
		DECLARE_SET(SetFloat4x4, glm::mat4x4, SHADER_VAR_TYPE_FLOAT, 4, 4)

#undef DECLARE_SET

		size_t GetSizeInBytes() const { return m_bufferLayout.m_size; }
		size_t GetAlignedSizeInBytes() const { return CalcNearestMultiple(m_bufferLayout.m_size, 64); }
		size_t GetElementCount() const { return m_bufferLayout.m_layout.size(); }
		int GetIndexFromName(std::string& name);
		ShaderVariableTypes GetType(size_t index) const { return m_bufferLayout.m_layout[index].m_type; }
	};
}