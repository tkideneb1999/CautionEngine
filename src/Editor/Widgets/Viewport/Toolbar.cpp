#include "Toolbar.h"

#include "imgui.h"
namespace Reckless
{
	void Toolbar::OnEditorLayerAttach()
	{
	}
	void Toolbar::DrawLayer()
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{ }
			if (ImGui::MenuItem("Exit"))
			{
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	void Toolbar::AddMenu(const char* name, const std::function<void()>& callback)
	{
		printf("Adding menu: %s", name);
	}
}