#pragma once

#include "IEditorLayer.h"

#include <functional>
#include <map>
#include <string>

namespace Reckless
{
	/// <summary>
	/// TODO: we need to make a callback system that you can simply just add menus and callbacks
	/// </summary>
	class CMainEditorToolbar : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void Update() override;

	public:
		// ! \brief Adds a custom tool menu
		// ! \param Function callback that will be invoked when the menu is executed
		void AddToToolsMenu(const char* name, const std::function<void()>& callback);

	private:
		using TPairMenuAction = std::pair<std::string, std::function<void()>>;
		std::vector<TPairMenuAction> m_dynamicMenus;
	};

DECLARE_SHARED_POINTERS(CMainEditorToolbar)
}
