#pragma once

#include <exception>
#include <string>
#include <wrl/client.h>

#include <dxcapi.h>
#include <d3d12shader.h>

#include "Shader.h"
#include "ShaderData.h"

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering {

	class D3D12ShaderCompiler 
	{
	private:
		static LPCWSTR GetShaderEntryPoint(ShaderStage stage) {
			switch (stage)
			{
			case SHADER_STAGE_VERTEX:
				return L"VSMain";
			case SHADER_STAGE_HULL:
				return L"HSMain";
			case SHADER_STAGE_DOMAIN:
				return L"DSMain";
			case SHADER_STAGE_GEOMETRY:
				return L"GSMain";
			case SHADER_STAGE_PIXEL:
				return L"PSMain";
			default:
				throw std::exception("No Valid Shader Stage provided!");
			}
		}

		static std::wstring GetShaderModel(ShaderStage stage, ShaderModel model) 
		{
			std::wstring shaderModelStr;

			switch (stage)
			{
			case SHADER_STAGE_VERTEX:
				shaderModelStr += L"vs_";
				break;
			case SHADER_STAGE_HULL:
				shaderModelStr += L"hs_";
				break;
			case SHADER_STAGE_DOMAIN:
				shaderModelStr += L"ds_";
				break;
			case SHADER_STAGE_GEOMETRY:
				shaderModelStr += L"gs_";
				break;
			case SHADER_STAGE_PIXEL:
				shaderModelStr += L"ps_";
				break;
			default:
				throw std::exception("No Valid Shader Stage provided!");
			}

			switch (model) 
			{
			case SHADER_MODEL_6_0:
				shaderModelStr += L"6_0";
				break;
			case SHADER_MODEL_6_1:
				shaderModelStr += L"6_1";
				break;
			case SHADER_MODEL_6_2:
				shaderModelStr += L"6_2";
				break;
			case SHADER_MODEL_6_3:
				shaderModelStr += L"6_3";
				break;
			case SHADER_MODEL_6_4:
				shaderModelStr += L"6_4";
				break;
			case SHADER_MODEL_6_5:
				shaderModelStr += L"6_5";
				break;
			case SHADER_MODEL_6_6:
				shaderModelStr += L"6_6";
				break;
			default:
				throw std::exception("No Valid Shader Model provided!");
			}

			return shaderModelStr;
		}

		static bool GetShaderVarType(const D3D_SHADER_VARIABLE_TYPE* dxcType, ShaderVariableTypes* engineType);

		ComPtr<IDxcCompiler3> m_compiler;
		ComPtr<IDxcIncludeHandler> m_includeHandler;
		ComPtr<IDxcUtils> m_utils;

		ComPtr<ID3D12ShaderReflection> m_reflectionData[SHADER_STAGE_COUNT];

		Shader* m_pShader;
		DxcBuffer m_shaderSource;

		bool CompileStage(ShaderStage stage);
		bool GenerateShaderData();

		DXGI_FORMAT GetInputElementFormat(D3D12_SIGNATURE_PARAMETER_DESC* reflectionData);

	public:
		D3D12ShaderCompiler() = delete;
		D3D12ShaderCompiler(Shader* shader);

		bool Compile();

		Shader* GetShader() { return m_pShader; }
	};
}