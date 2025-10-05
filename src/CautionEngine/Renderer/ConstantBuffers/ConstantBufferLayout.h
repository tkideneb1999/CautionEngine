#pragma once

#include <CautionEngineDefinitions.h>

#include <d3d12.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "../ShaderData.h"

// Forward Declaration
namespace CautionEngine::Rendering
{
	class DescriptorManager;
	class D3D12ShaderCompiler;
}

namespace CautionEngine::Rendering::ConstantBuffers
{
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
			{
			}
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

		ConstantBufferLayout(const ConstantBufferLayout& other);

		std::string const& GetName() const { return m_name; }
		bool IsEqual(ConstantBufferLayout const& other) const;
	};
}