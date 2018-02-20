#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "GameState.h"

class GameObject;

class GameStateManager
{
public:
	/// registers a new GameState with the given name.
	void registerState(const std::string& name, GameState::Ptr state);

	/// Changes to the given state
	void setState(const std::string& stateName);

	void update(float delta);
	void draw();
	void HandleEvent(Event event);

private:
	GameState* findState(const std::string& stateName);
	void changeState(GameState* stateName);

	std::unordered_map<std::string, GameState::Ptr> states;
	GameState* currentState = nullptr;
	GameState* futureState = nullptr;
};
