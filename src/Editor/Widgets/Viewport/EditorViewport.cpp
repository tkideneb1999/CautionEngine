#include "EditorViewport.h"

#include "imgui.h"

namespace Reckless
{
	void EditorViewport::OnEditorLayerAttach()
	{
	}
	void EditorViewport::DrawLayer()
	{
		ImGui::Begin("Viewport");
		ImGui::End();
	}
}
