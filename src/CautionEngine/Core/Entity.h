#pragma once

#include "BaseObject.h"

#include <Components/Component.h>

#include <string>
#include <vector>

struct ObjectID;
// !Namespaces..
using namespace CautionEngine::Components;

namespace CautionEngine::Core
{
	class Entity : public BaseObject
	{
	public:
		Entity();
		Entity(const char* name);

		inline const char* GetName() const { return m_name.c_str(); }

		void AddComponent(std::shared_ptr<Component> component)
		{
			// TODO: do more later...
			m_components.push_back(component);
		};

		Component* GetComponentByID(const ObjectID* id);

		template <typename ComponentType>
		ComponentType* GetComponentByClass() {
			// TODO: make a caster that will generated the ComponentID of the component
			return nullptr;
		}
		void RemoveComponent(Component* component);

		void Destroy();

	private:
		std::string m_name;
		std::vector<std::shared_ptr<Component>> m_components;
	};
}