#include "stdafx.h"
#include "EditorViewport.h"

namespace Reckless
{
	void CEditorViewport::OnEditorLayerAttach()
	{
	}
	void CEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		ImGui::End();
	}
}
