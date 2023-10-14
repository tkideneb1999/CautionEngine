#pragma once

namespace Reckless
{
	class IEditorLayer
	{
	public:
		virtual ~IEditorLayer() = default;

		virtual void OnEditorLayerAttach() { }
		virtual void OnEditorLayerDetach() { }

		virtual void DrawLayer() = 0;
		virtual void Update() { }
		virtual void OnRender() { }
	};
}