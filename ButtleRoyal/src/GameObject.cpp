#include "stdafx.h"

#include "GameObject.h"

void
GameObject::AddComponent(IComponent::Ptr component)
{
	m_componentList.push_back(component);
}

void GameObject::Update(const float fDeltaTime)
{
	for (auto comp : m_componentList)
		comp->VUpdate(fDeltaTime);

}

bool GameObject::Init()
{
	for (auto comp : m_componentList)
	{
		if (!comp->VInit())
		{
			err() << "Could not initialize component of object " << m_id << std::endl;
			return false;
		}
	}
	return true;
}