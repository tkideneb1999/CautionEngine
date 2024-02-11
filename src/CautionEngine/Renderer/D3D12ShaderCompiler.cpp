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
		std::vector<LPCWSTR> args = {
			filepath, // Filepath for debugging
			L"-E", GetShaderEntryPoint(stage),
			L"-T", GetShaderModel(stage, model).c_str(), // Shader Model (TODO Check if Get Shader Model works)
#if _DEBUG
			L"-Zs", // Debug Information
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

		return false;
	}
}