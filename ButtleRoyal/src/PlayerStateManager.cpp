#include "stdafx.h"
#include "PlayerStateManager.h"
#include <iostream>
#include "DebugDraw.h"


PlayerStateManager::PlayerStateManager(GameObject& player)
	:IComponent(player)
{	
}


void PlayerStateManager::registerState(const std::string& name, IPlayerState::Ptr state)
{
	m_states[name] = state;
}

void PlayerStateManager::setState(const std::string& stateName)
{
	IPlayerState* state = findState(stateName);
	FF_ASSERT_MSG(state != nullptr, "State could not be found");

	futureState = state;
}

std::string PlayerStateManager::getCurrentStateName()
{
	for (auto const& x : m_states)
	{
		if(x.second.get() == currentState)
		{
			return x.first;
		}
	}
	return "";
}

void PlayerStateManager::changeState(IPlayerState* state)
{
	if (state != currentState)
	{
		if (currentState != nullptr)
			currentState->VExit();		

		currentState = state;
	}
}

void PlayerStateManager::VUpdate(float delta)
{
	futureState = findState(currentState->VHandleInput(m_gameObject));

	if (futureState != nullptr)
	{
		if (!DebugDraw::getInstance().IsEnabled())
		{
			std::cout << "State changed to " << currentState->VHandleInput(m_gameObject) << std::endl;
		}
		
		changeState(futureState);
		futureState = nullptr;
	}

	if (currentState != nullptr)
		currentState->VUpdate(delta);
}

bool PlayerStateManager::VInit()
{
	currentState = findState("RightStandingState");
	return true;
}

void PlayerStateManager::draw()
{
	if (currentState != nullptr)
		currentState->VDraw();
}


IPlayerState* PlayerStateManager::findState(const std::string& stateName)
{
	auto state = m_states.find(stateName);
	if (state != m_states.end())
		return state->second.get();
	return nullptr;
}
