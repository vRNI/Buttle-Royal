#include "stdafx.h"

#include "GameStateManager.h"
#include "GameState.h"
#include "Debug.h"

void GameStateManager::registerState(const std::string& name, GameState::Ptr state)
{
	states[name] = state;
}

void GameStateManager::setState(const std::string& stateName)
{
	GameState* state = findState(stateName);
	FF_ASSERT_MSG(state != nullptr, "State could not be found");

	futureState = state;
}

void GameStateManager::changeState(GameState* state)
{
	if (state != currentState)
	{
		if (currentState != nullptr)
			currentState->VExit();		

		currentState = state;

		if (currentState != nullptr)
			currentState->VInit();
	}
}

void GameStateManager::update(float delta)
{
	if (futureState != nullptr)
	{
		changeState(futureState);
		futureState = nullptr;
	}

	if (currentState != nullptr)
		currentState->VUpdate(delta);
}

void GameStateManager::draw()
{
	if (currentState != nullptr)
		currentState->VDraw();
}
void GameStateManager::HandleEvent(Event event)
{
	if (currentState != nullptr)
		currentState->VHandleEvent(event);
}



GameState* GameStateManager::findState(const std::string& stateName)
{
	auto state = states.find(stateName);
	if (state != states.end())
		return state->second.get();
	return nullptr;
}
