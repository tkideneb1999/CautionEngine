#pragma once

#include <CautionDefinitions.h>

#include <exception>
#include <string>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	CAUTION_API inline void THROW_IF_FAILED(HRESULT res, const char* msg)
	{
		if (!SUCCEEDED(res))
		{
			throw std::exception(msg);
		}
	}

	inline std::string GetErrorMessageFromBlob(ID3DBlob* errorBlob)
	{
		return std::string((const char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
	}
}
