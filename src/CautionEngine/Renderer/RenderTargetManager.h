#pragma once

#include <CautionDefinitions.h>

#include <vector>

#include "Formats.h"
#include "DescriptorManager.h"
#include "RenderTarget.h"

namespace CautionEngine::Rendering 
{
	class CAUTION_CLASS RenderTargetManager
	{
	private:
		DescriptorManager* m_pDescriptorManager;

		// TODO: Better Allocation -> maybe heap allocate these
		std::unordered_map<unsigned int, RenderTarget> m_renderTargets;
	public:
		
		RenderTargetManager(DescriptorManager* pDescriptorManager);

		RenderTargetManager() = delete;
		RenderTargetManager(const RenderTargetManager&) = delete;

		unsigned int CreateRenderTarget(unsigned int width, unsigned int height, RenderFormat format, unsigned int mipLevel = 1, const float* clearValue = nullptr);
		RenderTarget* GetRenderTarget(unsigned int id);

		void Shutdown();

	private:
		void CreateRenderTargetResource(
			unsigned int width, unsigned int height, RenderFormat format, unsigned int mipLevels, bool isDepthRT,
			const float* pClearValue, Microsoft::WRL::ComPtr<ID3D12Resource>& pRenderTargetResource
		);

		unsigned int GetFreeID();
	};
}