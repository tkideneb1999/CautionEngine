#pragma once 

#include "../EditorLayer.h"

namespace Reckless
{
	class Toolbar : public EditorLayer
	{
	public:
		virtual void OnEditorLayerAttach() override;
	};
}