#pragma once

#include "../EditorLayer.h"

namespace Reckless
{
	class EditorViewport : public EditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
	};
}