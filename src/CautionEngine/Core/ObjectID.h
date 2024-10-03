#pragma once
#include "stdafx.h"
#include <CautionDefinitions.h>

struct CAUTION_CLASS CObjectID
{
	CObjectID() {}

	static CObjectID Generate()
	{
		// TODO: implement...
		return CObjectID();
	}

	static CObjectID FromString(const std::string& objectIdStr)
	{
		return CObjectID();
	}
};