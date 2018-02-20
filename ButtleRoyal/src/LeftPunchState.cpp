
#include "stdafx.h"
#include "LeftPunchState.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "BoxColliderComponent.h"


LeftPunchState::LeftPunchState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string LeftPunchState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_PUNCHLEFT);
	
	if(player.GetComponent<SpriteAnimationComponent>()->GetIsFullyLooped())
	{

		player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
		return "LeftStandingState";
		
	}
	
	return "";
}