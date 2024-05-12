#pragma once

#include <IEditorLayer.h>

namespace Reckless
{
	class CProperyEditor : public IEditorLayer
	{
	public:


		// Inherited via IEditorLayer
		void Update() override;

	};

DECLARE_SHARED_POINTERS(CProperyEditor);
}