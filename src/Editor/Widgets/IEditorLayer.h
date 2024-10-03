#pragma once

#include <Platform/CautionPlatform.h>

#include "imgui.h"

namespace Reckless
{
	struct IEditorLayer
	{
		IEditorLayer() = default;
		virtual ~IEditorLayer() = default;

		virtual void OnEditorLayerAttach() { }
		virtual void OnEditorLayerDetach() { }

		virtual void Update() = 0;
	};
}