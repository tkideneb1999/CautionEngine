#include "stdafx.h"
#include "ShaderManager.h"

#include "D3D12ShaderCompiler.h"

namespace CautionEngine::Rendering
{
	Shader* ShaderManager::CreateShader(const std::string& filePath)
	{
		const size_t hashValue = m_hash(filePath);
		if (m_shaders.contains(hashValue))
		{
			// Shader already exists, TODO: Take care of Shader Mutations or recompilation
			return nullptr;
		}
		std::wstring wStringFilePath(filePath.begin(), filePath.end());
		Shader* pShader = new Shader(wStringFilePath.c_str());
		D3D12ShaderCompiler compiler(pShader, m_pCBufferManager);
		if (!compiler.Compile())
		{
			// Shader could not be compiled, do something with error here
			return nullptr;
		}

		m_shaders.insert(std::pair<size_t, Shader*>(hashValue, pShader));
		return m_shaders[hashValue];
	}

	Shader* ShaderManager::GetShader(std::string_view& filePath) const
	{
		const size_t hashValue = m_hash(filePath);
		if (auto shaderEntry = m_shaders.find(hashValue); shaderEntry != m_shaders.end())
		{
			return shaderEntry->second;
		}
		return nullptr;
	}

	ShaderManager::~ShaderManager()
	{
		for (const std::pair<size_t, Shader*>& shaderEntry : m_shaders)
		{
			delete shaderEntry.second;
		}
	}
}
