#include "StaticMeshComponent.h"

namespace CautionEngine::Components
{
	StaticMeshComponent::StaticMeshComponent()
	{
		// SAMPLE
		//m_componentId = ObjectID("{8FA7036C-A4C7-4BD9-8C40-872184FDE7C2}")
		m_id = ObjectID::Generate();
	}
}