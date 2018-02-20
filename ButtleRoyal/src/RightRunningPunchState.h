#pragma once

#include "IPlayerState.h"

class RightRunningPunchState : public IPlayerState
{
public:
	RightRunningPunchState(std::shared_ptr<PlayerStateManager> manager);

	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
