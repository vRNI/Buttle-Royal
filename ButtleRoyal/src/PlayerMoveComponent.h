#pragma once

#include "IComponent.h"

#include "RigidBodyComponent.h"

class PlayerMoveComponent : public IComponent
{
public:
	typedef std::shared_ptr<PlayerMoveComponent> Ptr;

	PlayerMoveComponent(GameObject &gameObject, RigidBodyComponent& rigidBody, int playerIndex);

	virtual bool VInit() override;
	virtual void VUpdate(float fDeltaTime) override;

	int GetPlayerIndex() const { return m_iPlayerIndex; }

private:
	int m_iPlayerIndex;
	RigidBodyComponent& m_rigidBody;
};