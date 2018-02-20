
#include "stdafx.h"
#include "LeftJumpingState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


LeftJumpingState::LeftJumpingState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftJumpingState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_LANDINGLEFT);
	player.GetComponent<PlayerPropertyComponent>()->setHasJumped(true);
	player.GetComponent<PlayerPropertyComponent>()->setOnGround(false);

	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()) ||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "LeftAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x > 1.2f)
	{
		return "RightJumpingState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -300.f)
	{
		return "LeftAirbornState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 100.f)
	{
		return "LeftLandingState";
	}
	
	if(player.GetComponent<PlayerPropertyComponent>()->isOnGround())
	{
		if(player.GetComponent<RigidBodyComponent>()->GetVelocity().x < 1.2f)
		{
			return "LeftStandingState";
		}
		return "LeftRunningState";
	}

	return "";
}