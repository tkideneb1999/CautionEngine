#pragma once

#include <CautionDefinitions.h>

#include <exception>
#include <string>
#include <wrl/client.h>

using namespace Microsoft::WRL;

#define SAFE_RELEASE(p) { if ((p)) {(p).Reset();} }

namespace CautionEngine::Rendering
{
	CAUTION_CLASS inline void THROW_IF_FAILED(HRESULT res, const char* msg)
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

	inline D3D12_RESOURCE_BARRIER CreateResourceBarrierTransition(
		ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after,
		UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE
	)
	{
		D3D12_RESOURCE_BARRIER result{};
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		result.Flags = flags;
		result.Transition.pResource = pResource;
		result.Transition.StateBefore = before;
		result.Transition.StateAfter = after;
		result.Transition.Subresource = subresource;

		return result;
	}
}
