#include "stdafx.h"
#include "D3D12ShaderCompiler.h"

#include <vector>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering {
	bool D3D12ShaderCompiler::GenerateShaderData()
	{
		// Get VS Inputs
		ComPtr<ID3D12ShaderReflection> vsReflection = m_reflectionData[SHADER_STAGE_VERTEX];

		D3D12_SHADER_DESC shaderDescs[SHADER_STAGE_COUNT];
		for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
			if (m_reflectionData[i] == nullptr)
				continue;
			m_reflectionData[i]->GetDesc(&(shaderDescs[i]));
		}

		// Get Vertex Shader Inputs
		D3D12_SHADER_DESC* vsShaderDesc = &shaderDescs[SHADER_STAGE_VERTEX];
		m_pShader->m_InputElementDescs = std::vector<D3D12_INPUT_ELEMENT_DESC>();
		m_pShader->m_InputElementDescs.reserve(vsShaderDesc->InputParameters);

		m_pShader->m_vsInputs = std::vector<ShaderInput>(vsShaderDesc->InputParameters);
		for (unsigned int i = 0; i < vsShaderDesc->InputParameters; i++) {
			D3D12_SIGNATURE_PARAMETER_DESC inputParameterDesc{};
			vsReflection->GetInputParameterDesc(i, &inputParameterDesc);

			// Get if float, float2, etc.
			unsigned short int paramAmount = 0;
			if (inputParameterDesc.Mask == 0b000'0001)
				paramAmount = 1;
			else if (inputParameterDesc.Mask == 0b000'0011)
				paramAmount = 2;
			else if (inputParameterDesc.Mask == 0b000'0111)
				paramAmount = 3;
			else if (inputParameterDesc.Mask == 0b000'1111)
				paramAmount = 4;
			
			// Get Semantic
			if (strcmp(inputParameterDesc.SemanticName, "POSITION") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_POSITION;
			else if (strcmp(inputParameterDesc.SemanticName, "NORMAL") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_NORMAL;
			else if (strcmp(inputParameterDesc.SemanticName, "COLOR") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_COLOR;
			else if (strcmp(inputParameterDesc.SemanticName, "TANGENT") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_TANGENT;
			else if (strcmp(inputParameterDesc.SemanticName, "BITANGENT") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_BITANGENT;
			else if (strcmp(inputParameterDesc.SemanticName, "TEXCOORD") == 0)
				m_pShader->m_vsInputs[i].semantic = SHADER_INPUT_TEXCOORD;
			else
			{
				std::cout << "Semantic " << inputParameterDesc.SemanticName << " is not supported" << std::endl;
				return false;
			}
			m_pShader->m_InputElementDescs.push_back({
				.SemanticName = inputParameterDesc.SemanticName,
				.SemanticIndex = inputParameterDesc.SemanticIndex,
				.Format = GetInputElementFormat(&inputParameterDesc),
				.InputSlot = 0,
				.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT,
				.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0,
			});
			
			// TODO: Maybe remove this if not needed
			m_pShader->m_vsInputs[i].semanticIndex = inputParameterDesc.SemanticIndex;
			m_pShader->m_vsInputs[i].type = (ShaderVariableTypes)inputParameterDesc.ComponentType;
			m_pShader->m_vsInputs[i].amount = paramAmount;
		}

		// Get Root Parameters
		std::unordered_map<std::string, unsigned int>* shaderRootParamIndexMap = &m_pShader->m_rootParameterIndexMap;
		shaderRootParamIndexMap->clear();
		std::vector<D3D12_ROOT_PARAMETER1> rootParams = {};

		for (int si = 0; si < SHADER_STAGE_COUNT; si++) {
			if (m_reflectionData[si] == nullptr)
				continue;
			ComPtr<ID3D12ShaderReflection> stageReflection = m_reflectionData[si];
			D3D12_SHADER_DESC* stageDesc = &(shaderDescs[si]);
			for (unsigned int ri = 0; ri < stageDesc->BoundResources; ri++) {
				D3D12_SHADER_INPUT_BIND_DESC resourceBindDesc;
				stageReflection->GetResourceBindingDesc(ri, &resourceBindDesc);
				std::string resourceName = std::string(resourceBindDesc.Name);

				// Constant Buffers
				if (resourceBindDesc.Type == D3D_SIT_CBUFFER) {
					
					if (shaderRootParamIndexMap->contains(resourceName))
						continue;
					ID3D12ShaderReflectionConstantBuffer* cbufferReflection = stageReflection->GetConstantBufferByIndex(ri);

					// Get Variables in Buffer
					D3D12_SHADER_BUFFER_DESC bufferDesc;
					cbufferReflection->GetDesc(&bufferDesc);

					// Buffer Validation (TODO: Skip when not in Editor)

					std::vector<D3D12ConstantBufferVariable> cbufferElements = std::vector<D3D12ConstantBufferVariable>();

					for (int vi = 0; vi < bufferDesc.Variables; vi++)
					{
						ID3D12ShaderReflectionVariable* var = cbufferReflection->GetVariableByIndex(vi);
						D3D12_SHADER_VARIABLE_DESC varDesc;
						
						var->GetDesc(&varDesc);
						ID3D12ShaderReflectionType* varType = var->GetType();
						D3D12_SHADER_TYPE_DESC varTypeDesc;
						varType->GetDesc(&varTypeDesc);

						ShaderVariableTypes cBufferVarType = {};

						if (GetShaderVarType(&varTypeDesc.Type, &cBufferVarType))
						{
							D3D12ConstantBufferVariable bufferLayoutVar = {
							.name = std::string(varDesc.Name),
							.type = cBufferVarType,
							.count = (unsigned short int)(varTypeDesc.Columns * varTypeDesc.Rows),
							.offset = (unsigned short int)varDesc.StartOffset,
							};

							cbufferElements.push_back(bufferLayoutVar);
						}
						else if (varTypeDesc.Type == D3D_SVT_VOID && varTypeDesc.Class == D3D_SVC_STRUCT)
						{
							std::cout << "Buffers defined via ConstantBuffer<Struct> currently not supported" << std::endl;
							break;
						}
						else
							throw std::exception("Shader Variable type currently not supported");
					}

					D3D12ConstantBufferLayout bufferLayout =
					{
						.elements = cbufferElements,
						.name = resourceName,
					};

					// Create Root Param
					D3D12_ROOT_PARAMETER1 cbvRootParam
					{
						.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
						.Descriptor {
							.ShaderRegister = resourceBindDesc.BindPoint,
							.RegisterSpace = resourceBindDesc.Space,
							.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE,
						},
					};
					(*shaderRootParamIndexMap)[resourceName] = rootParams.size();
					rootParams.push_back(cbvRootParam);
					m_pShader->m_bufferLayouts.push_back(bufferLayout);
					std::cout << "Found CBuffer: " << resourceBindDesc.Name << std::endl;
				}
				else if (resourceBindDesc.Type == D3D_SIT_TEXTURE) {
					//TODO: Implement
					std::cout << "Found Texture: " << resourceBindDesc.Name << std::endl;
				}
				else if (resourceBindDesc.Type == D3D_SIT_SAMPLER) {
					//TODO: Implement
					std::cout << "Found Sampler: " << resourceBindDesc.Name << std::endl;
				}
			}
		}
		
		// Generate Root Signature
		// TODO: Extend this with Textures & Descriptor Tables
		D3D12_ROOT_PARAMETER1* pData = nullptr;
		if (rootParams.size() > 0)
			pData = rootParams.data();
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {
			.Version = D3D_ROOT_SIGNATURE_VERSION_1_1,
			.Desc_1_1 =
			{
				.NumParameters = (UINT)rootParams.size(),
				.pParameters = pData,
				.NumStaticSamplers = 0,
				.pStaticSamplers = nullptr,
				.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
			},
		};
		ComPtr<ID3DBlob> errors;
		HRESULT res = D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &m_pShader->m_serializedRootSignature, &errors);
		if (FAILED(res))
		{
			std::cout << "Root Signature Serialization failed:\n" << GetErrorMessageFromBlob(errors.Get()) << std::endl;
		}
				
		return true;
	}

	DXGI_FORMAT D3D12ShaderCompiler::GetInputElementFormat(D3D12_SIGNATURE_PARAMETER_DESC* reflectionData)
	{
		DXGI_FORMAT result = DXGI_FORMAT_UNKNOWN;

		// Note: Theoretically the DGXI Formats follow the order of: FLOAT, UINT, SINT and are offsetted by the
		// respective amount of channels, but this feels to unsafe, if the order in the DGXI Format changes
		switch (reflectionData->ComponentType)
		{
			case D3D_REGISTER_COMPONENT_FLOAT32:
				switch (reflectionData->Mask)
				{
					case 0b000'0001:
						result = DXGI_FORMAT_R32_FLOAT;
						break;
					case 0b000'0011:
						result = DXGI_FORMAT_R32G32_FLOAT;
						break;
					case 0b000'0111:
						result = DXGI_FORMAT_R32G32B32_FLOAT;
						break;
					case 0b000'1111:
						result = DXGI_FORMAT_R32G32B32A32_FLOAT;
						break;
				}
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				switch (reflectionData->Mask)
				{
					case 0b000'0001:
						result = DXGI_FORMAT_R32_UINT;
						break;
					case 0b000'0011:
						result = DXGI_FORMAT_R32G32_UINT;
						break;
					case 0b000'0111:
						result = DXGI_FORMAT_R32G32B32_UINT;
						break;
					case 0b000'1111:
						result = DXGI_FORMAT_R32G32B32A32_UINT;
						break;
				}
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				switch (reflectionData->Mask)
				{
				case 0b000'0001:
					result = DXGI_FORMAT_R32_SINT;
					break;
				case 0b000'0011:
					result = DXGI_FORMAT_R32G32_SINT;
					break;
				case 0b000'0111:
					result = DXGI_FORMAT_R32G32B32_SINT;
					break;
				case 0b000'1111:
					result = DXGI_FORMAT_R32G32B32A32_SINT;
					break;
				}
				break;
		}
		if (result == DXGI_FORMAT_UNKNOWN)
			throw std::exception("Input Element Format Unkown/Not Supported");

		return result;
	}

	D3D12ShaderCompiler::D3D12ShaderCompiler(Shader* shader)
	{
		m_pShader = shader;
		m_shaderSource = DxcBuffer();

		THROW_IF_FAILED(
			DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler)),
			"Compiler creation failed!"
		);
		THROW_IF_FAILED(
			DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_utils)),
			"Couldn't create Utils!"
		);
		THROW_IF_FAILED(
			m_utils->CreateDefaultIncludeHandler(&m_includeHandler),
			"Couldn't create Include Handler"
		);

		for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
			m_reflectionData[i] = ComPtr<ID3D12ShaderReflection>();
		}
	}

	bool D3D12ShaderCompiler::Compile()
	{
		// Load Shader
		ComPtr<IDxcBlobEncoding> encodingSource = nullptr;
		m_utils->LoadFile(m_pShader->GetFilepath(), nullptr, &encodingSource);
		m_shaderSource.Ptr = encodingSource->GetBufferPointer();
		m_shaderSource.Size = encodingSource->GetBufferSize();
		m_shaderSource.Encoding = DXC_CP_ACP;

		//Compile Shader Stages
		bool succeeded = true;
		for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
			succeeded = succeeded && CompileStage((ShaderStage)i);
		}
		if (m_pShader->m_stageShaders[SHADER_STAGE_VERTEX] == nullptr && m_pShader->m_stageShaders[SHADER_STAGE_PIXEL] == nullptr)
		{
			std::cout << "INVALID SHADER: Vertex and/or Pixel Shader could not be found!" << std::endl;
			return false;
		}

		if (!succeeded)
			return false;

		if (!GenerateShaderData())
			return false;

		return true;
	}

	bool D3D12ShaderCompiler::GetShaderVarType(const D3D_SHADER_VARIABLE_TYPE* dxcType, ShaderVariableTypes* engineType)
	{
		if (*dxcType == D3D_SVT_FLOAT)
			*engineType = SHADER_VAR_TYPE_FLOAT;
		else if (*dxcType == D3D_SVT_BOOL)
			*engineType = SHADER_VAR_TYPE_BOOL;
		else if (*dxcType == D3D_SVT_INT)
			*engineType = SHADER_VAR_TYPE_INT;
		else if (*dxcType == D3D_SVT_UINT)
			*engineType = SHADER_VAR_TYPE_UINT;
		else
			return false;
		return true;
	}

	bool D3D12ShaderCompiler::CompileStage(ShaderStage stage)
	{
		// Construct args
		// TODO: Decide what to do with ShaderModel
		std::wstring shaderModel = GetShaderModel(stage, SHADER_MODEL_6_0);
		std::vector<LPCWSTR> args = {
			m_pShader->GetFilepath(), // Filepath for debugging
			L"-E", GetShaderEntryPoint(stage),
			L"-T", shaderModel.c_str(), // Shader Model (TODO Check if Get Shader Model works)
#if _DEBUG
			L"-Zi", // Debug Information
			L"-Od", // Disable Optimizations
#endif
			L"-Qstrip_reflect", // Don't bake reflection into Shader Object
		};

		ComPtr<IDxcResult> results;
		HRESULT res = m_compiler->Compile(
			&m_shaderSource,
			args.data(),
			args.size(),
			m_includeHandler.Get(),
			IID_PPV_ARGS(&results)
		);

		HRESULT hrStatus;
		results->GetStatus(&hrStatus);
		if (FAILED(hrStatus))
		{
			ComPtr<IDxcBlobUtf8> errors = nullptr;
			results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);

			// Check if shader stage has entry point if not return true
			if (strcmp(errors->GetStringPointer(), "error: missing entry point definition")) {
				return true;
			}

			// Print other errors
			std::wcout << L"Compilation Error:\n" << errors->GetStringPointer() << std::endl;
			return false;
		}

		//Reflection
		ComPtr<IDxcBlob> pReflectionDataSource = {};
		results->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&pReflectionDataSource), nullptr);
		if (pReflectionDataSource == nullptr)
		{
			throw std::exception("reflection Data null");
		}

		DxcBuffer stageReflectionData;
		stageReflectionData.Encoding = DXC_CP_ACP;
		stageReflectionData.Ptr = pReflectionDataSource->GetBufferPointer();
		stageReflectionData.Size = pReflectionDataSource->GetBufferSize();

		m_utils->CreateReflection(&stageReflectionData, IID_PPV_ARGS(&(m_reflectionData[stage])));

		// ShaderObject
		// TODO: Serialize to disk and deserialize into shader object
		results->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&m_pShader->m_stageShaders[stage]), nullptr);

		return true;
	}
}