#include "Scene.h"
namespace CautionEngine::Core
{
	Scene::Scene()
		: Scene("new_scene")
	{
		// TODO: initializations
	}

	Scene::Scene(const char* name)
		: m_sceneName(name)
	{
	}
}