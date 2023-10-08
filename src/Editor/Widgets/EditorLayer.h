#pragma once

namespace Reckless
{
	class EditorLayer
	{
	public:
		virtual ~EditorLayer() = default;

		virtual void OnEditorLayerAttach() { }
		virtual void OnEditorLayerDetach() { }

		virtual void Update() { }
		virtual void OnRender() { }
	};
}