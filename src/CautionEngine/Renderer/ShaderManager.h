#pragma once

#include <CautionEngineDefinitions.h>
#include <memory>

#include "Shader.h"
#include "D3D12ShaderCompiler.h"

namespace CautionEngine::Rendering {

	// Forward Declaration
	namespace ConstantBuffers
	{
		class ConstantBufferManager;
	}

	class CAUTION_ENGINE_CLASS ShaderManager 
	{
	private:
		std::hash<std::string_view> m_hash;
		std::unordered_map<size_t, Shader*> m_shaders;
		std::shared_ptr<ConstantBuffers::ConstantBufferManager> const m_pCBufferManager;

	public:
		ShaderManager(std::shared_ptr<ConstantBuffers::ConstantBufferManager> pCBufferManager)
			: m_hash()
			, m_shaders()
			, m_pCBufferManager(pCBufferManager)
		{}

		~ShaderManager();

		Shader* CreateShader(const std::string& filePath);
		Shader* GetShader(std::string_view& filepath) const;
	};
}
