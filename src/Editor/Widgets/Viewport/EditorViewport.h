#pragma once

#include "IEditorLayer.h"

namespace Reckless
{
	class CEditorViewport : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void DrawLayer() override;
	};

	DECLARE_SHARED_POINTERS(CEditorViewport)
}