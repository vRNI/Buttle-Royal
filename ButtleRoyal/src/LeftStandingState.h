#pragma once

#include "GameObject.h"
#include "IPlayerState.h"

class LeftStandingState : public IPlayerState
{
public:
	LeftStandingState(std::shared_ptr<PlayerStateManager> manager);

	void VInit() override {}
	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VDraw() override{}
	void VExit() override {}
};
