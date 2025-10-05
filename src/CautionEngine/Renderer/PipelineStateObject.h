#pragma once

#include <CautionEngineDefinitions.h>
#include <d3d12.h>

#include <wrl/client.h>

#include "ShaderData.h"
#include "Shader.h"

using namespace Microsoft::WRL;

namespace CautionEngine::Rendering
{
	class PipelineStateObject
	{
	private:
		ComPtr<ID3D12PipelineState> m_d3d12Pso;
		Shader* m_pShader = nullptr;

		ShaderTopologyType m_topologyType = SHADER_TOPOLOGY_TRIANGLE;
		D3D12_PRIMITIVE_TOPOLOGY m_IATopologyType = D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// RTV Formats


		// Depth & Stencil Behaviour
		bool m_enableDepthTesting = true;
		ShaderDepthComp m_depthComp = SHADER_DEPTH_COMP_LESS_EQUAL;

		// Rasterizer State
		ShaderFillMode m_fillMode = SHADER_FILL_SOLID;
		ShaderCullMode m_cullMode = SHADER_CULL_NONE;
		bool m_doConservativeRaster = false;

		//Blend State
		bool m_doIndividualBlend = false;
		bool m_alphaToCoverage = false;
		std::vector<BlendInfo> m_blendInfos;
		
		inline D3D12_RENDER_TARGET_BLEND_DESC CreateD3D12BlendDesc(BlendInfo* pBlendInfo);

	public:
		PipelineStateObject();

		void Generate();
		void Serialize();
		void Deserialize();

		void SetShader(Shader* pShader) { m_pShader = pShader; }
		const Shader* const GetShader() const { return m_pShader; }

		bool GetEnableDepthTesting() const { return m_enableDepthTesting; }
		void SetEnableDepthTesting(bool enable) { m_enableDepthTesting = enable; }

		ShaderDepthComp GetDepthCompFunction() const { return m_depthComp; }
		void SetDepthCompFunction(ShaderDepthComp comp) { m_depthComp = comp; }

		void SetTopologyType(ShaderTopologyType type);
		ShaderTopologyType GetTopologyType() const { return m_topologyType; }

		void SetFillMode(ShaderFillMode fillMode) { m_fillMode = fillMode; }
		ShaderFillMode GetFillMode() const { return m_fillMode; }

		void SetCullMode(ShaderCullMode cullMode) { m_cullMode = cullMode; }
		ShaderCullMode GetCullMode() const { return m_cullMode; }

		void SetDoConservativeRaster(bool doConservativeRaster) { m_doConservativeRaster = doConservativeRaster; }
		bool GetDoConservativeRaster() { return m_doConservativeRaster; }

		void SetState(ID3D12GraphicsCommandList* pCommandList);
		ID3D12PipelineState* GetD3D12PipelineState() const { return m_d3d12Pso.Get(); }
	};
}