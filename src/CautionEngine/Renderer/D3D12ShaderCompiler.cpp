#include "stdafx.h"
#include "D3D12ShaderCompiler.h"

#include <vector>

#include "D3D12Helpers.h"

namespace CautionEngine::Rendering {

	bool D3D12ShaderCompiler::CompileShader(LPCWSTR filepath, ShaderStage stage, ShaderModel model) 
	{
		ComPtr<IDxcCompiler3> compiler;
		ComPtr<IDxcUtils> utils;

		// Create Compiler
		ThrowIfFailed(
			DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)),
			"Compiler creation failed!"
		);
		ThrowIfFailed(
			DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)),
			"Couldn't create Utils!"
		);
		
		// Create Include Handler (TODO)
		ComPtr<IDxcIncludeHandler> includeHandler;
		utils->CreateDefaultIncludeHandler(&includeHandler);

		// Load Shader
		ComPtr<IDxcBlobEncoding> encodingSource = nullptr;
		utils->LoadFile(filepath, nullptr, &encodingSource);
		DxcBuffer shaderSource;
		shaderSource.Ptr = encodingSource->GetBufferPointer();
		shaderSource.Size = encodingSource->GetBufferSize();
		shaderSource.Encoding = DXC_CP_ACP;
		// Generate Args
		std::wstring shaderModel = GetShaderModel(stage, model);
		std::vector<LPCWSTR> args = {
			filepath, // Filepath for debugging
			L"-E", GetShaderEntryPoint(stage),
			L"-T", shaderModel.c_str(), // Shader Model (TODO Check if Get Shader Model works)
#if _DEBUG
			L"-Zi", // Debug Information
			L"-Od", // Disable Optimizations
#endif
		};

		// Compile Shader
		ComPtr<IDxcResult> results;
		HRESULT res = compiler->Compile(
			&shaderSource,
			args.data(),
			args.size(),
			includeHandler.Get(),
			IID_PPV_ARGS(&results)
		);

		ComPtr<IDxcBlobUtf8> errors = nullptr;
		results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
		if (errors != nullptr && errors->GetStringLength() != 0)
		{
			wprintf(L"Warnings & Errors:\n%S\n", errors->GetStringPointer());
			return false;
		}
		return true;
	}
}