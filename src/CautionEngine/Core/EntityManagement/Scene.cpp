#include "stdafx.h"

#include "Scene.h"

namespace CautionEngine::Core
{
	CScene::CScene()
		: CScene("new_scene")
	{
		// TODO: initializations
	}

	CScene::CScene(const char* name)
		: m_sceneName(name)
	{
	}
	void CScene::Render()
	{
	}
	void CScene::Destroy()
	{
	}
}