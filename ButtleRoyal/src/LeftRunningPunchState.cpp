
#include "stdafx.h"
#include "LeftRunningPunchState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


LeftRunningPunchState::LeftRunningPunchState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftRunningPunchState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_PUNCHLEFT);

	if(player.GetComponent<SpriteAnimationComponent>()->GetIsFullyLooped())
	{
		player.GetComponent<RigidBodyComponent>()->SetVelocity(Vector2f(0.0f, 0.0f));
		player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
		return "LeftStandingState";
		
	}
	
	return "";
}