#pragma once

#include <CautionDefinitions.h>
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
		Shader* m_pShader;

	public:
		void Serialize();
		void Deserialize();

		void SetShader(Shader* pShader) { m_pShader = pShader; }
		const Shader* const GetShader() const { return m_pShader; }

		void Generate();
	};
}