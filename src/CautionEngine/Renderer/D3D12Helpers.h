#pragma once

#include <CautionDefinitions.h>

#include <exception>
#include <wrl/client.h>

using namespace Microsoft::WRL;

CAUTION_API inline void ThrowIfFailed(HRESULT res, const char* msg)
{
	if (!SUCCEEDED(res))
	{
		throw std::exception(msg);
	}
}