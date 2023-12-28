#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
#include "wrl/client.h"

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	class Shader
	{
	private:
		const LPCWSTR m_filepath;

		ComPtr<ID3DBlob> m_vertexShader;
		ComPtr<ID3DBlob> m_pixelShader;

#if _DEBUG
		static const UINT m_compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		static const UINT m_compileFlags = 0;
#endif

	public:
		Shader() = delete;
		Shader(const LPCWSTR shaderFilepath);

		void Compile();

	private:
		bool CompileShader(const char* Function, const char* targetShaderModel, ID3DBlob** ppCompiledShader);
	};
}