#pragma once

#include <memory>

class GameObject;

class IComponent
{
public:
	typedef std::shared_ptr<IComponent> Ptr;

	IComponent(GameObject& gameObject)
		: m_gameObject(gameObject)
	{
	}

	virtual ~IComponent() = default;

	virtual bool VInit() = 0;
	virtual void VUpdate(float fDeltaTime) = 0;

protected:

	GameObject& m_gameObject;
};