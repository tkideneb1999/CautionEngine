#include "stdafx.h"
#include "ConstantBufferLayout.h"

namespace CautionEngine::Rendering::ConstantBuffers
{
	bool ConstantBufferLayout::AppendData(
		unsigned int dataSize, std::string& name, ShaderVariableTypes type, unsigned int columns, unsigned int rows
	)
	{
		if (m_nameMapping.find(name) != m_nameMapping.end())
		{
			return false;
		}
		m_nameMapping.insert(std::pair<std::string, size_t>(name, m_layout.size()));
		m_layout.emplace_back(dataSize, m_size + dataSize, type, columns, rows);
		m_size += dataSize;
		return true;
	}

	bool ConstantBufferLayout::IsEqual(ConstantBufferLayout const& other)
	{
		if (m_name != other.m_name)
		{
			return false;
		}

		return true;
	}
}