#include "stdafx.h"
#include "Shader.h"

#include <exception>
#include <sstream>
#include <iostream>

#include "PipelineStateObject.h"

namespace CautionEngine::Rendering
{
	void Shader::Serialize()
	{
		//TODO: Serialize ShaderBytecode
		//TODO: Serialize RootSignature
		//TODO: Serialize Bindings (Textures, Samplers)
	}

	void Shader::Deserialize()
	{
		//TODO: Load ShaderByteCode

	}

	bool Shader::GetStageShaderCode(const void** ppBuffer, size_t* pLength, ShaderStage stage)
	{
		if (m_stageShaders[stage] == nullptr)
			return false;
		*ppBuffer = m_stageShaders[stage]->GetBufferPointer();
		*pLength = m_stageShaders[stage]->GetBufferSize();
		return true;
	}

	bool Shader::SetShaderStageInPSODesc(D3D12_SHADER_BYTECODE* pByteCodeStructure, ShaderStage stage)
	{
		if (m_stageShaders[stage] == nullptr)
			return false;
		pByteCodeStructure->pShaderBytecode = m_stageShaders[stage]->GetBufferPointer();
		pByteCodeStructure->BytecodeLength = m_stageShaders[stage]->GetBufferSize();
		return true;
	}

	Shader::Shader(const LPCWSTR shaderFilepath)
		:m_filepath(shaderFilepath), m_rootParameterIndexMap()
	{
		m_vsInputs = std::vector<ShaderInput>();
		for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
			m_stageShaders[i] = ComPtr<ID3DBlob>();
		}
	}
}