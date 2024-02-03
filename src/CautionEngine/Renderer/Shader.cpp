#include "stdafx.h"
#include "Shader.h"

#include <exception>
#include <sstream>
#include <iostream>

namespace CautionEngine::Rendering
{
	Shader::Shader(const LPCWSTR shaderFilepath)
		:m_filepath(shaderFilepath)
	{
		Compile();
	}

	void Shader::Compile()
	{
		CompileShader("VSMain", "vs_5_0", &m_vertexShader);
		CompileShader("PSMain", "ps_5_0", &m_pixelShader);
		std::wcout << "Successfully compiled Shader:\n    " << m_filepath << std::endl;
	}

	bool Shader::CompileShader(const char* function, const char* targetShaderModel, ID3DBlob** ppCompiledShader)
	{
		ComPtr<ID3DBlob> errorMessageBlob;
		HRESULT res = D3DCompileFromFile(
			m_filepath, nullptr, nullptr, function, targetShaderModel, m_compileFlags, 0, ppCompiledShader, &errorMessageBlob
		);
		if (res == E_FAIL)
		{
			char* errorMessageStart = (char*)errorMessageBlob->GetBufferPointer();
			SIZE_T errorMessageSize = errorMessageBlob->GetBufferSize();
			std::stringstream errorMessageStream{};
			for (SIZE_T i = 0; i < errorMessageSize; i++)
			{
				char* currentCharPtr = errorMessageStart + i;
				errorMessageStream << *currentCharPtr;
			}
			std::cout << errorMessageStream.str() << std::endl;
			throw std::exception("Couldn't compile Shader");
		}
		return true;
	}
}