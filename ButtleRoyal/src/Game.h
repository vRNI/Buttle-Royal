#pragma once

#include <string>

#include "GameStateManager.h"

#include "PlayerPropertyManager.h"
#include "SoundManager.h"				

class InputManager;
class DebugDraw;

class Game
{
public:
	struct Config
	{
		Vector2i resolution{ 1920, 1080 };
		std::string windowName = "FinalFrontier";
	};
	
	Config &getConfig() { return m_Config; }
	RenderWindow &getWindow() { return m_window;  }

	void Run();

private:

	bool init();
	void update();
	void draw();
	void shutdown() const;

	Config m_Config;
	// Feedback: Gibt es einen grund warum inkonsestent für die Manager mal Pointer mal Referenzen verwendet wurden?
	RenderWindow m_window;
	GameStateManager m_gameStateManager;
	PlayerPropertyManager* m_playerPropertyManager;
	InputManager* m_inputManager;
	SoundManager* m_soundManager;	
	DebugDraw* m_debugDraw;
};
