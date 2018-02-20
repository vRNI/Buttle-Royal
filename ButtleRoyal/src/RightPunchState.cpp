
#include "stdafx.h"
#include "RightPunchState.h"
#include "GameStateManager.h"
#include "SpriteAnimationComponent.h"
#include "GameObjectManager.h"


RightPunchState::RightPunchState(const std::shared_ptr<PlayerStateManager> manager)
	: IPlayerState(manager)
{
}

std::string RightPunchState::VHandleInput(GameObject& player)
{
	player.GetComponent<SpriteAnimationComponent>()->ChangeAnimationTo(SpriteAnimationComponent::STATE_PUNCHRIGHT);

	if (player.GetComponent<SpriteAnimationComponent>()->GetIsFullyLooped())
	{

		player.GetComponent<SpriteAnimationComponent>()->SetIsFullyLooped(false);
		return "RightStandingState";
	}
	
	return "";
}