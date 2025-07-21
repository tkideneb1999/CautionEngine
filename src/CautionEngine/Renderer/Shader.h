#pragma once

#include <CautionEngineDefinitions.h>

#include <vector>
#include <string>
#include <d3d12.h>
#include <unordered_map>
#include "wrl/client.h"

#include "ShaderData.h"

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	class Shader
	{
		friend class D3D12ShaderCompiler;
	private:
		const LPCWSTR m_filepath;

		ComPtr<ID3DBlob> m_stageShaders[SHADER_STAGE_COUNT];

		std::unordered_map<std::string, unsigned int> m_rootParameterIndexMap;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3DBlob> m_serializedRootSignature;

		std::vector<ShaderInput> m_vsInputs;
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputElementDescs;
		std::vector<std::string> m_semanticNames;

		std::vector<unsigned int> m_cbufferIDs;

	public:
		Shader() = delete;
		Shader(const Shader& other) = delete;
		CAUTION_ENGINE_CLASS Shader(const LPCWSTR shaderFilepath);

		CAUTION_ENGINE_CLASS void Serialize();
		CAUTION_ENGINE_CLASS void Deserialize();

		bool GetStageShaderCode(const void** ppBuffer, size_t* pLength, ShaderStage stage);
		ID3D12RootSignature* GetRootSignature() { return m_rootSignature.Get(); }
		const std::vector<D3D12_INPUT_ELEMENT_DESC>* GetInputLayoutDescs() { return &m_InputElementDescs; }

		CAUTION_ENGINE_CLASS const LPCWSTR GetFilepath() { return m_filepath; }

	private:
		bool SetShaderStageInPSODesc(D3D12_SHADER_BYTECODE* pByteCodeStructure, ShaderStage stage);
	};
}
