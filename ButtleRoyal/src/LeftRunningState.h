#pragma once

#include "IPlayerState.h"

class LeftRunningState : public IPlayerState
{
public:
	LeftRunningState(std::shared_ptr<PlayerStateManager> manager);

	void VInit() override {}
	void VDraw() override {}
	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
