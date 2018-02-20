#pragma once

#pragma once

#include <list>
#include <string>

#include "IComponent.h"

class GameObject: public Transformable
{
public:
	typedef std::shared_ptr<GameObject> Ptr;

	GameObject(const std::string &id)
		: m_id(id)
	{}

	/// Add a component to the game object. Internally the game object
	/// identifies if it's a drawable component to call it's draw
	/// function or a general update component to call it's update
	/// function.
	/// \param component the component
	void AddComponent(IComponent::Ptr component);

	/// Returns the component of the given type
	template<typename TComponent>
	std::shared_ptr<TComponent> GetComponent() {
		for (auto component: m_componentList) {
			std::dynamic_pointer_cast<TComponent>(component);
			if (std::shared_ptr<TComponent> cmp = std::dynamic_pointer_cast<TComponent>(component)) {
				return cmp;
			}
		}
		return nullptr;
	}

	/// Initialize all added components
	bool Init();

	/// Could also be moved out of game object in the long run.
	void Update(float fDeltaTime);

	bool IsMarkedForDelete() const { return m_wantToDie; }
	void MarkForDelete() { m_wantToDie = true; }

	std::string GetId() const { return m_id; }
	void SetId(const std::string id) { m_id = id; }

	void SetType(const std::string type) { m_type = type; }
	std::string GetType() { return m_type; }


protected:
	std::string m_id = "unnamed"; //< unique name of object, e.g., player
	std::string m_type = "unnamed";
	bool m_wantToDie = false;
	std::list<IComponent::Ptr> m_componentList;
};