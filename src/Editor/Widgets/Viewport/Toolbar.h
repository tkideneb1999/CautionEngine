#pragma once 

#include "../IEditorLayer.h"

namespace Reckless
{
	class Toolbar : public IEditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
		virtual void DrawLayer() override;
	};
}