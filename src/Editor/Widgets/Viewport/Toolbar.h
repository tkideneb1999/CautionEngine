#pragma once 

#include "../IEditorLayer.h"

#include <functional>

namespace Reckless
{
	/// <summary>
	/// TODO: we need to make a callback system that you can simply just add menus and callbacks
	/// </summary>
	class Toolbar : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void DrawLayer() override;

	public:
		void AddMenu(const char* name, const std::function<void()>& callback);
	};

DECLARE_SHARED_POINTERS(Toolbar)
}
