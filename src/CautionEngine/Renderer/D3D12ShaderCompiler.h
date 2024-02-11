#pragma once

#include <map>
#include <exception>
#include <string>
#include <wrl/client.h>

#include <dxcapi.h>
#include <d3d12shader.h>

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering {
	enum CAUTION_API ShaderStage {
		SHADER_STAGE_VERTEX = 0,
		SHADER_STAGE_HULL = 1,
		SHADER_STAGE_DOMAIN = 2,
		SHADER_STAGE_GEOMETRY = 3,
		SHADER_STAGE_PIXEL = 4,
	};

	enum CAUTION_API ShaderModel {
		SHADER_MODEL_6_0 = 0,
		SHADER_MODEL_6_1 = 1,
		SHADER_MODEL_6_2 = 2,
		SHADER_MODEL_6_3 = 3,
		SHADER_MODEL_6_4 = 4,
		SHADER_MODEL_6_5 = 5,
		SHADER_MODEL_6_6 = 6,
	};

	class CAUTION_API D3D12ShaderCompiler {
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

	public:
		D3D12ShaderCompiler() = delete;

		static bool CompileShader(LPCWSTR filepath, ShaderStage stage, ShaderModel model);
	};
}