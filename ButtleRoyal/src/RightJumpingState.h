#pragma once

#include "GameObject.h"
#include "IPlayerState.h"

class RightJumpingState : public IPlayerState
{
public:
	RightJumpingState(std::shared_ptr<PlayerStateManager> manager);

	void VInit() override {}
	void VDraw() override {}
	std::string VHandleInput(GameObject& player) override;
	void VUpdate(float delta)override {}
	void VExit() override {}
};
