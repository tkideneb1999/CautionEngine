#include "stdafx.h"
#include "PipelineStateObject.h"
#include "D3D12API.h"
#include "D3D12Helpers.h"

#include <exception>

namespace CautionEngine::Rendering {
	PipelineStateObject::PipelineStateObject()
	{
		m_blendInfos = std::vector<BlendInfo>(1);
	}

	inline D3D12_RENDER_TARGET_BLEND_DESC PipelineStateObject::CreateD3D12BlendDesc(BlendInfo* pBlendInfo)
	{
		return D3D12_RENDER_TARGET_BLEND_DESC{
			.BlendEnable = pBlendInfo->useBlend,
			.LogicOpEnable = pBlendInfo->useLogicalOp,
			.SrcBlend = (D3D12_BLEND)pBlendInfo->srcBlend,
			.DestBlend = (D3D12_BLEND)pBlendInfo->destBlend,
			.BlendOp = (D3D12_BLEND_OP)pBlendInfo->blendOp,
			.SrcBlendAlpha = (D3D12_BLEND)pBlendInfo->srcBlendAlpha,
			.DestBlendAlpha = (D3D12_BLEND)pBlendInfo->destBlendAlpha,
			.BlendOpAlpha = (D3D12_BLEND_OP)pBlendInfo->blendOpAlpha,
			.LogicOp = (D3D12_LOGIC_OP)pBlendInfo->logicOp,
			.RenderTargetWriteMask = pBlendInfo->RenderTargetWriteMask,
		};
	}

	void PipelineStateObject::Generate()
	{
		if (m_pShader == nullptr)
			throw new std::exception("No Shader Set");

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		// Set Shader Stage Bytecode
		m_pShader->GetStageShaderCode(
			&psoDesc.VS.pShaderBytecode, &psoDesc.VS.BytecodeLength, SHADER_STAGE_VERTEX
		);
		m_pShader->GetStageShaderCode(
			&psoDesc.HS.pShaderBytecode, &psoDesc.HS.BytecodeLength, SHADER_STAGE_HULL
		);
		m_pShader->GetStageShaderCode(
			&psoDesc.DS.pShaderBytecode, &psoDesc.DS.BytecodeLength, SHADER_STAGE_DOMAIN
		);
		m_pShader->GetStageShaderCode(
			&psoDesc.GS.pShaderBytecode, &psoDesc.GS.BytecodeLength, SHADER_STAGE_GEOMETRY
		);
		m_pShader->GetStageShaderCode(
			&psoDesc.PS.pShaderBytecode, &psoDesc.PS.BytecodeLength, SHADER_STAGE_PIXEL
		);

		// Set Root Signature
		psoDesc.pRootSignature = m_pShader->GetRootSignature();

		// Set Input Layout
		// - Validate Input Layout later on
		const std::vector<D3D12_INPUT_ELEMENT_DESC>* pInputs = m_pShader->GetInputLayoutDescs();
		psoDesc.InputLayout.pInputElementDescs = pInputs->data();
		psoDesc.InputLayout.NumElements = pInputs->size();

		// Set RenderTarget Data (from Renderer)
		// - DSVFormats
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // TODO: Make this not hard coded
		// - RTVFormats
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		
		// - Index Buffer
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		
		// - SampleDesc (Multi Sample)
		psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		
		// - DepthStencil State TODO
		psoDesc.DepthStencilState.DepthEnable = false; // TODO: Make this available
		//psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		//psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

		psoDesc.DepthStencilState.StencilEnable = false; // TODO: Make this optional
		
		// - StreamOutput (for readback from GPU, can be ignored for now)

		// Set Material Parameters
		// - Blend state
		psoDesc.BlendState.IndependentBlendEnable = m_doIndividualBlend;
		psoDesc.BlendState.AlphaToCoverageEnable = m_alphaToCoverage;
		for (int i = 0; i < m_blendInfos.size(); i++)
		{
			psoDesc.BlendState.RenderTarget[i] = CreateD3D12BlendDesc(&(m_blendInfos[i]));
		}

		// - Rasterizer State
		psoDesc.RasterizerState.FillMode = (D3D12_FILL_MODE)m_fillMode;
		psoDesc.RasterizerState.CullMode = (D3D12_CULL_MODE)m_cullMode;
		if (m_doConservativeRaster)
			psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON;
		else
			psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		// - Topology type
		psoDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)m_topologyType;
		D3D12API* const pApi = D3D12API::Get();
		THROW_IF_FAILED(
			pApi->GetDevicePtr()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_d3d12Pso)),
			"Failed to create PSO"
		);
	}
	void PipelineStateObject::SetTopologyType(ShaderTopologyType type)
	{
		m_topologyType = type;
		switch (type)
		{
		case SHADER_TOPOLOGY_LINE:
			m_IATopologyType = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case SHADER_TOPOLOGY_TRIANGLE:
			m_IATopologyType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		case SHADER_TOPOLOGY_POINT:
			m_IATopologyType = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case SHADER_TOPOLOGY_PATCH:
			m_IATopologyType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; // TODO: Check this
			break;
		}
	}
	void PipelineStateObject::SetState(ID3D12GraphicsCommandList* pCommandList)
	{
		pCommandList->SetGraphicsRootSignature(m_pShader->GetRootSignature());
		pCommandList->IASetPrimitiveTopology(m_IATopologyType);
		pCommandList->SetPipelineState(m_d3d12Pso.Get());
	}
}
