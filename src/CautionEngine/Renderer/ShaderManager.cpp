#include "stdafx.h"
#include "ShaderManager.h"

#include "D3D12ShaderCompiler.h"

void CautionEngine::Rendering::ShaderManager::CreateShader(LPCWSTR filePath)
{
	Shader shader(filePath);
	D3D12ShaderCompiler compiler(&shader);
	compiler.Compile();
}
