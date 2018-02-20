
#include "stdafx.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "LeftRunningState.h"


LeftRunningState::LeftRunningState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftRunningState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_WALKLEFT);
	
	if(player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 50)
	{
		return "LeftJumpingState";
	}

	if(abs(player.GetComponent<RigidBodyComponent>()->GetVelocity().x) < 1.2f)
	{
		return "LeftStandingState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x > 1.2f)
	{
		return "RightRunningState";
	}

	if(InputManager::getInstance().isKeyDown("punch",player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "LeftPunchState";
	}

	if (player.GetComponent<PlayerPropertyComponent>()->isOnGround() &&
		InputManager::getInstance().isKeyDown("jump", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex())||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "LeftJumpingState";
	}

	return "";
}