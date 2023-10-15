#pragma once

#include "BaseObject.h"

#include <string>
#include <vector>

struct ObjectID;

namespace CautionEngine::Core
{
	class Component;
	/*class BaseObject;*/
	class Entity : public BaseObject
	{
	public:
		Entity();
		Entity(const char* name);

		const char* GetName() const { return m_name.c_str(); }

		inline void AddComponent(Component* component)
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
		std::vector<Component*> m_components;
	};
}