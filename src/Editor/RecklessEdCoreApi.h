#pragma once

#include "Widgets/Application.h"
#include "Widgets/RecklessEditors.h" // TODO: might not be a good idea to do this...

#include <Core/EntityManagement/EntityManager.h>

static Reckless::CWinApplication* s_recklessEditor;

Reckless::CWinApplication* const GetRecklessEditor()
{
	return s_recklessEditor;
}

static CautionEngine::Core::CEntityManager* s_entityManager;

CautionEngine::Core::CEntityManager* const GetEntityManager()
{
	return s_entityManager;
}
