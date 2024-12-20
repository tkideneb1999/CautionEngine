#pragma once

#include <CautionEngineDefinitions.h>

#include "Shader.h"
#include "D3D12ShaderCompiler.h"

namespace CautionEngine::Rendering {

	class CAUTION_ENGINE_CLASS ShaderManager 
	{
	private:
		std::hash<std::string_view> m_hash;
		std::unordered_map<size_t, Shader*> m_shaders;

	public:
		ShaderManager()
			:m_hash()
			,m_shaders()
		{}

		~ShaderManager();

		Shader* CreateShader(const std::string& filePath);
		Shader* GetShader(std::string_view& filepath) const;
	};
}