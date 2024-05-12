#include "stdafx.h"

#include "DescriptorManager.h"

namespace CautionEngine::Rendering 
{
	void DescriptorManager::Init(
		D3D12API* pApi, int cbv_srv_uav_count, int dsv_count, int rtv_count, int sampler_count)
	{
		m_cbv_srv_uav_descHeap = D3D12::DescriptorHeap(
			pApi->GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, cbv_srv_uav_count, true);
		m_dsv_descHeap = D3D12::DescriptorHeap(
			pApi->GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, dsv_count, true);
		m_rtv_descHeap = D3D12::DescriptorHeap(
			pApi->GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rtv_count, true);
		m_sampler_descHeap = D3D12::DescriptorHeap(
			pApi->GetDevicePtr().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, sampler_count, true);
	}
	void DescriptorManager::SetShaderVisibleDescriptorHeaps(ID3D12GraphicsCommandList* commandList)
	{
		ID3D12DescriptorHeap* ppDescriptorHeaps[] = { m_cbv_srv_uav_descHeap.GetHeapPtr().Get(), m_sampler_descHeap.GetHeapPtr().Get() };
		commandList->SetDescriptorHeaps(_countof(ppDescriptorHeaps), ppDescriptorHeaps);
	}
}