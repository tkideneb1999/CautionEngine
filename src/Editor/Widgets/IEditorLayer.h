#pragma once

#include <Platform/CautionPlatform.h>

#include "imgui.h"

namespace Reckless
{
	class IEditorLayer
	{
	public:
		IEditorLayer() = default;
		virtual ~IEditorLayer() = default;

		virtual void OnEditorLayerAttach() { }
		virtual void OnEditorLayerDetach() { }

		virtual void DrawLayer() = 0;
		virtual void Update() { }
	};
}