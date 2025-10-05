#include "stdafx.h"
#include "BaseObject.h"

namespace CautionEngine::Core
{
	CObject::CObject()
	{
		/*m_id = */
		uint64_t id = SObjectID::Generate();
		// TODO: proper logging
		printf("generated id: %d \n", (int)id);
	}
}