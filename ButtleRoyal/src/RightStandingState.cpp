
#include "stdafx.h"
#include "RightStandingState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightStandingState::RightStandingState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightStandingState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_IDLERIGHT);
	player.GetComponent<PlayerPropertyComponent>()->setOnGround(true);
	
	if(abs(player.GetComponent<RigidBodyComponent>()->GetVelocity().x) > 1.2f)
	{
		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x > 1.2f)
			return "RightRunningState";

		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x < -1.2f)
			return "LeftRunningState";
	}

	if(InputManager::getInstance().isKeyDown("punch",player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "RightPunchState";
	}

	if (player.GetComponent<PlayerPropertyComponent>()->isOnGround() &&
		InputManager::getInstance().isKeyDown("jump", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("jump", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "RightJumpingState";
	}

	return "";
}


