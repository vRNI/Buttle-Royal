#pragma once

#include "stdafx.h"
#include "GameObject.h"
#include "Animation.h"

class PlayerStateManager;

class IPlayerState
{
public:
	virtual ~IPlayerState() = default;
	typedef std::shared_ptr<IPlayerState> Ptr;

	/// \brief this is the interface for the states in the statemachine
	/// \param manager is given so changes can be changed from there
	IPlayerState(const std::shared_ptr<PlayerStateManager> manager)
	:m_manager(manager)
	{}

	virtual void VInit() = 0;
	/// \brief the statechanging-logic is in VHandleInput of the inheriting states
	virtual std::string VHandleInput(GameObject& player) = 0;
	virtual void VUpdate(float delta) = 0;
	virtual void VDraw() = 0;
	virtual void VExit() = 0;

protected:
	std::vector<Animation> m_stateframes;
	std::shared_ptr<PlayerStateManager> m_manager;
};
