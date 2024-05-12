#pragma once

#include <CautionDefinitions.h>

#include "D3D12API.h"
#include "D3D12DescriptorHeap.h"

namespace CautionEngine::Rendering
{

	class CAUTION_API DescriptorManager
	{
	private:
		D3D12::DescriptorHeap m_cbv_srv_uav_descHeap;
		D3D12::DescriptorHeap m_rtv_descHeap;
		D3D12::DescriptorHeap m_dsv_descHeap;
		D3D12::DescriptorHeap m_sampler_descHeap;



	public:
		DescriptorManager() {};
		void Init(
			D3D12API* pApi,
			int cbv_srv_uav_count, 
			int dsv_count, 
			int rtv_count, 
			int sampler_count);

		D3D12::DescriptorHeapHandle AllocateCbvSrvUav() { return m_cbv_srv_uav_descHeap.Allocate(); }
		D3D12::DescriptorHeapHandle AllocateRTV() { return m_rtv_descHeap.Allocate(); }
		D3D12::DescriptorHeapHandle AllocateDSV() { return m_dsv_descHeap.Allocate(); }
		D3D12::DescriptorHeapHandle AllocateSampler() { return m_sampler_descHeap.Allocate(); }

		D3D12::DescriptorHeap* GetCbvSrvUavHeap() { return &m_cbv_srv_uav_descHeap; }
		D3D12::DescriptorHeap* GetRTVHeap() { return &m_rtv_descHeap; }
		D3D12::DescriptorHeap* GetDSVHeap() { return &m_dsv_descHeap; }
		D3D12::DescriptorHeap* GetSamplerHeap() { return &m_sampler_descHeap; }

		void SetShaderVisibleDescriptorHeaps(ID3D12GraphicsCommandList* commandList);
	};
}