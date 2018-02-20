#pragma once

#include "IPlayerState.h"

class RightRunningState : public IPlayerState
{
public:
	RightRunningState(std::shared_ptr<PlayerStateManager> manager);

	void VInit() override {}
	void VDraw() override {}
	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
