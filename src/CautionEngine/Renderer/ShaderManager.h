#pragma once

#include <CautionDefinitions.h>

#include "Shader.h"
#include "D3D12ShaderCompiler.h"

namespace CautionEngine::Rendering {

	class ConstantBufferManager;

	class CAUTION_CLASS ShaderManager 
	{
	private:
		std::hash<std::string_view> m_hash;
		std::unordered_map<size_t, Shader*> m_shaders;
		ConstantBufferManager* const m_pCBufferManager;

	public:
		ShaderManager(ConstantBufferManager* const pCBufferManager)
			: m_hash()
			, m_shaders()
			, m_pCBufferManager(pCBufferManager)
		{}

		~ShaderManager();

		Shader* CreateShader(const std::string& filePath);
		Shader* GetShader(std::string_view& filepath) const;
	};
}