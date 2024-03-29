#pragma once

#include <CautionDefinitions.h>

#include <vector>

#include "Formats.h"
#include "DescriptorManager.h"
#include "RenderTarget.h"

namespace CautionEngine::Rendering 
{
	class RenderTargetManager
	{
	private:
		DescriptorManager* m_pDescriptorManager;

		std::vector<RenderTarget> m_renderTargets;
	public:
		
		RenderTargetManager(DescriptorManager* pDescriptorManager);

		RenderTargetManager() = delete;
		RenderTargetManager(const RenderTargetManager&) = delete;

		RenderTarget* CreateRenderTarget(unsigned int width, unsigned int height, RenderFormat format);
	};
}