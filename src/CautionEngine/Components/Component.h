#pragma once

#include <Core/ObjectID.h>
#include <CautionDefinitions.h>

class CEntity;

namespace CautionEngine::Components
{
	class CAUTION_API CEntityComponent
	{
	public:
		CEntityComponent();

		virtual void Register();
		virtual void Unregister();

		virtual void Update() = 0;
		virtual void Destroy() = 0;

		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;

		const CObjectID& GetID() const { return m_id; }

		CEntity*         GetOwner() const { return m_pOwner; }
		inline void      SetOwner(CEntity* owner) { m_pOwner = owner; }
	
	protected:
		friend CEntity;

		CObjectID         m_componentId;
		CObjectID         m_id;
		CEntity*          m_pOwner;
	};
}