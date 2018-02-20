#pragma once

#include "ButtleRoyal/dll_export.h"

#include <memory>

class GameStateManager;
class Game;

class FINALFRONTIER_API GameState
{
public:
	typedef std::shared_ptr<GameState> Ptr;
	
	GameState(GameStateManager* gameStateManager, Game* game)
		: m_gameStateManager(gameStateManager)
		, m_game(game)
	{}
	virtual ~GameState() = default;

	/// called when game state is entered.
	virtual void VInit() {}

	/// called when game state is left.
	virtual void VExit() {}

	virtual void VUpdate(float delta) = 0;
	virtual void VDraw() = 0;
	virtual void VHandleEvent(Event event) = 0;

protected:
	GameStateManager* m_gameStateManager;
	Game* m_game;
};
