#pragma once

#include "BaseObject.h"

#include <Components/Component.h>

struct CObjectID;
// !Namespaces..
using namespace CautionEngine::Components;

namespace CautionEngine::Core
{
	class CAUTION_API CEntity : public CObject
	{
	public:
		CEntity();
		CEntity(const char* name);

		CEntity operator=(CEntity&& other);

		inline const char* GetComponentName() const { return m_name.c_str(); }

		inline void AddComponent(std::shared_ptr<CEntityComponent> pComponent)
		{
			m_components.emplace_back(pComponent);
		};

		CEntityComponent* GetComponentByID(const CObjectID* id);

		template <typename T>
		// ! \brief Templated function to get Component by Class
		// ! \returns shared_ptr<T> component pointer
		std::shared_ptr<T> GetComponentByClass()
		{
			for (const std::shared_ptr<CEntityComponent> component : m_components)
			{
				if (std::shared_ptr<T> componentPtr = std::dynamic_pointer_cast<T>(component))
				{
					if (componentPtr)
						return componentPtr;
				}
			}
			return nullptr;
		}

		bool RemoveComponent(CEntityComponent * pComponentToRemove);

		void Destroy();

	private:
		std::string m_name;
		std::vector<std::shared_ptr<CEntityComponent>> m_components;
	};
}