#pragma once

#include "../Platform/CautionPlatform.h"

namespace CautionEngine
{
struct CAUTION_COMMON_API ICautionEngine
{
	ICautionEngine() {}

	void InitializeEngine();
	void ShutdownEngine();
};
}