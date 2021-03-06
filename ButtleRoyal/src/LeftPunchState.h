#pragma once

#include "GameObject.h"
#include "IPlayerState.h"

class LeftPunchState : public IPlayerState
{
public:
	LeftPunchState(std::shared_ptr<PlayerStateManager> manager);

	void VInit() override {}
	void VDraw() override {}
	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
