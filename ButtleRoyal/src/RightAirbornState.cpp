
#include "stdafx.h"
#include "RightAirbornState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"
#include "PlayerPropertyComponent.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightAirbornState::RightAirbornState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightAirbornState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_AIRBORNRIGHT);

	if (InputManager::getInstance().isKeyDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()) ||
		InputManager::getInstance().isButtonDown("punch", player.GetComponent<PlayerMoveComponent>()->GetPlayerIndex()))
	{
		return "RightAirPunchState";
	}

	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().x < -1.2f)
	{
		return "LeftAirbornState";
	}
	
	if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 200.f)
	{
		return "RightLandingState";
	}
	
	if(player.GetComponent<PlayerPropertyComponent>()->isOnGround())
	{
		if(abs(player.GetComponent<RigidBodyComponent>()->GetVelocity().x) < 1.2f)
		{
			return "RightStandingState";
		}
		return "RightRunningState";
	}
	
	return "";
}