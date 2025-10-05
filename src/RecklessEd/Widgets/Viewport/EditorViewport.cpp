#include "stdafx.h"
#include "EditorViewport.h"

#include <Renderer/RenderTargetManager.h>

namespace Reckless
{
	void CEditorViewport::OnEditorLayerAttach()
	{
		//m_sceneTextureId = m_pApp->GetSceneRenderTextureId();
	}

	void CEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		//RenderTarget* pTarget = m_pApp->GetRenderer()->GetRenderTargetManager()->GetRenderTarget(m_sceneTextureId);
		//ImGui::Image((ImTextureID)pTarget->descriptorHeapHandle.gpuHandle.ptr, ImVec2((float)pTarget->GetHeight(), (float)pTarget->GetWidth()));
		ImGui::End();
	}
}
