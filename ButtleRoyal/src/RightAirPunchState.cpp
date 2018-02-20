
#include "stdafx.h"
#include "RightAirPunchState.h"
#include "RigidBodyComponent.h"
#include "InputManager.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightAirPunchState::RightAirPunchState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightAirPunchState::VHandleInput(GameObject& player)
{
	//Auf airpunch stellen
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_PUNCHRIGHT);

	if(player.GetComponent<SpriteAnimationComponent>()->GetIsFullyLooped())
	{
		
		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -300.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "RightAirbornState";
		}

		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > 100.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "RightLandingState";
		}

		if (player.GetComponent<RigidBodyComponent>()->GetVelocity().y > -600.f)
		{
			player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
			return "Jumpingstate";
		}
		
	}
	
	return "";
}