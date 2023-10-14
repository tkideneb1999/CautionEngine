#pragma once

#include "../IEditorLayer.h"

namespace Reckless
{
	class AssetBrowserEditor : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void DrawLayer() override;
	};
}