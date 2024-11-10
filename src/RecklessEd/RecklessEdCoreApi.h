#pragma once

#include "Widgets/Application.h"
#include "Widgets/RecklessEditors.h" // TODO: might not be a good idea to do this...

static Reckless::CWinApplication* s_recklessEditor;

// !brief Gets the static instance of the RecklesEd
Reckless::CWinApplication* GetRecklessEditor()
{
	return s_recklessEditor;
}