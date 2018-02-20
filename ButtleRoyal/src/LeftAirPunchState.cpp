
#include "stdafx.h"
#include "LeftAirPunchState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


LeftAirPunchState::LeftAirPunchState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftAirPunchState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_PUNCHLEFT);

	if(player.GetComponent<SpriteAnimationComponent>()->GetIsFullyLooped())
	{
		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -300.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "LeftAirbornState";
		}

		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 100.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "LeftLandingState";
		}

		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -600.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "Jumpingstate";
		}
		
	}
	
	return "";
}