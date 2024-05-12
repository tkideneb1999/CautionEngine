#include "MainEditorToolbar.h"

namespace Reckless
{
	void CMainEditorToolbar::OnEditorLayerAttach()
	{
	}

	void CMainEditorToolbar::Update()
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{ }
			if (ImGui::MenuItem("Exit"))
			{
				/*GetRecklessEditor()->Shutdown();*/
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			for (const auto& subMenu : m_dynamicMenus)
			{
				if (ImGui::MenuItem(subMenu.first.c_str()))
				{
					// Menu callback/command
					subMenu.second();
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	void CMainEditorToolbar::AddToToolsMenu(const char* name, const std::function<void()>& callback)
	{
		printf("Adding menu: %s", name);
		m_dynamicMenus.emplace_back(std::make_pair(name, callback));
	}
}