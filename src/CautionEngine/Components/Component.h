#pragma once

#include <Core/ObjectID.h>
#include <CautionEngineDefinitions.h>

class CEntity;

namespace CautionEngine::Components
{
	class CAUTION_ENGINE_CLASS CEntityComponent
	{
	public:
		CEntityComponent() = default;

		virtual void Register();
		virtual void Unregister();

		virtual void Update() { }
		virtual void Destroy() { }

		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;

		const SObjectID& GetID() const { return m_id; }

		CEntity* GetEntityOwner() const { return m_pOwner; }
		inline void SetOwner(CEntity* owner) { m_pOwner = owner; }
	protected:
		friend CEntity;
		SObjectID         m_componentId;
		SObjectID         m_id;
		CEntity*          m_pOwner;
	};
}