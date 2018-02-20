#pragma once

#include "GameObject.h"
#include "IPlayerState.h"

class LeftRunningPunchState : public IPlayerState
{
public:
	LeftRunningPunchState(std::shared_ptr<PlayerStateManager> manager);

	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
