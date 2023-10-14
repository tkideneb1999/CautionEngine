#pragma once

#include "../IEditorLayer.h"

namespace Reckless
{
	class EditorViewport : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void DrawLayer() override;
	};
}