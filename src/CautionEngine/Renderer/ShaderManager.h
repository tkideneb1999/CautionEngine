#pragma once

#include <CautionDefinitions.h>

#include "Shader.h"
#include "D3D12ShaderCompiler.h"

namespace CautionEngine::Rendering {

	class CAUTION_API ShaderManager 
	{

	public:
		ShaderManager() {}
		void CreateShader(LPCWSTR filePath);
	};
}