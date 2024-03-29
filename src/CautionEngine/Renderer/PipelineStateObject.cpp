#include "stdafx.h"
#include "PipelineStateObject.h"

#include <exception>

void CautionEngine::Rendering::PipelineStateObject::Generate()
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
	// - RTVFormats, DSVFormats
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // TODO: Make this not hard coded
	
	// Set Material Parameters
	// - Blend state
	// - Rasterizer State
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	// - Topology type
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // Hardcoded for now

}
