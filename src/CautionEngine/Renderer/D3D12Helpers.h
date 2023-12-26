#pragma once

#include <exception>

#include <wrl/client.h>

using namespace Microsoft::WRL;

static void ThrowIfFailed(HRESULT res, const char* msg)
{
	if (!SUCCEEDED(res))
	{
		throw std::exception(msg);
	}
}