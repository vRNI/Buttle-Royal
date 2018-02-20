#include "stdafx.h"

#include "Game.h"

#include "InputManager.h"
#include "MenuState.h"
#include "MainState.h"
#include "DebugDraw.h"
#include "WinState.h"
#include "ControlsState.h"
#include "CreditState.h"
#include "LevelState.h"

using namespace std;


void Game::Run()
{
	if (!init())
		return;

	while (m_window.isOpen())
	{
		// process events in the input manager
		Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				m_window.close();
			else
			{
				m_inputManager->process(event);
				m_gameStateManager.HandleEvent(event);
			}			
		}

		update();
		draw();
	}

	shutdown();
}
vector<bool> CheckController()
{
	vector<bool> validControllers = vector<bool>(8);
	for (int i = 0; i < 8; i++)
	{
		if (Joystick::isConnected(i))
			validControllers[i] = true;
		else
			validControllers[i] = false;		
	}
	return validControllers;
}
bool Game::init()
{
	m_soundManager = &SoundManager::getInstance();
	m_soundManager->RegisterSoundBuffer("music", "../assets/sounds/Popskyy-Nintendisco.ogg");
	m_soundManager->RegisterSoundBuffer("button","../assets/sounds/button_press.wav");
	m_soundManager->RegisterSoundBuffer("jump1", "../assets/sounds/Jump1.wav");
	m_soundManager->RegisterSoundBuffer("jump2", "../assets/sounds/Jump2.wav");
	m_soundManager->RegisterSoundBuffer("hitResponse1", "../assets/sounds/Hit_Hurt1.wav");
	m_soundManager->RegisterSoundBuffer("hitResponse2", "../assets/sounds/Hit_Hurt2.wav");
	m_soundManager->RegisterSoundBuffer("Death1", "../assets/sounds/Death1.wav");
	m_soundManager->RegisterSoundBuffer("Death2", "../assets/sounds/Death2.wav");
	SoundManager::getInstance().PlaySound("music");
	

	//sf::Music music;
	//music.openFromFile("Popskyy - Nintendisco.ogg");
	//music.play();

	m_inputManager = &InputManager::getInstance();

	//Default Controller binding
	m_inputManager->bindController(0, 0);
	
	m_inputManager->bind("Exit", 6, 0, true);
	m_inputManager->bind("Select", 7, 0, true);
	m_inputManager->bind("jump", 0, 0, true);
	m_inputManager->bind("punch", 2, 0, true);

	m_inputManager->bindController(1, 1);
	m_inputManager->bind("Exit", 6, 1, true);
	m_inputManager->bind("Select", 7, 1, true);
	m_inputManager->bind("jump", 0, 1, true);
	m_inputManager->bind("punch", 2, 1, true);
	
	//Check for Valid Controllers and assign them to a player
	Joystick::update();
	auto valid = CheckController();
	int playerIdx = 0;
	for (int i =0;i <8;i++)
	{		
		if(valid[i])
		{
			std::cout << "bind controller " << i << " to playerIdx: " << playerIdx << std::endl;
			m_inputManager->bindController(i, playerIdx);
			m_inputManager->bind("Exit", 6, playerIdx, true);
			m_inputManager->bind("Select", 7, playerIdx, true);
			m_inputManager->bind("jump", 0, playerIdx, true);
			m_inputManager->bind("punch", 2, playerIdx, true);
			playerIdx++;
		}			
	}
	
	//KeyBoard SetUp
	m_inputManager->bind("Exit", Keyboard::Escape);
	m_inputManager->bind("Select", Keyboard::Space);

	m_inputManager->bind("left", Keyboard::Left, 0);
	m_inputManager->bind("right", Keyboard::Right, 0);
	m_inputManager->bind("jump", Keyboard::Up, 0);
	m_inputManager->bind("punch", Keyboard::Down, 0);
	m_inputManager->bind("change", Keyboard::Numpad0, 0);

	m_inputManager->bind("left", Keyboard::A, 1);
	m_inputManager->bind("right", Keyboard::D, 1);
	m_inputManager->bind("jump", Keyboard::W, 1);
	m_inputManager->bind("punch", Keyboard::S, 1);
	m_inputManager->bind("change", Keyboard::Q, 1);


	m_inputManager->bind("debug", Keyboard::F1,2);

	m_debugDraw = &DebugDraw::getInstance();
	//
	m_gameStateManager.registerState("MenuState", make_shared<MenuState>(&m_gameStateManager, this));
	m_gameStateManager.registerState("MainState", make_shared<MainState>(&m_gameStateManager, this));
	m_gameStateManager.registerState("WinState", make_shared<WinState>(&m_gameStateManager, this));
	m_gameStateManager.registerState("ControlsState", make_shared<ControlsState>(&m_gameStateManager, this));
	m_gameStateManager.registerState("CreditState", make_shared<CreditState>(&m_gameStateManager, this));
	m_gameStateManager.registerState("LevelState", make_shared<LevelState>(&m_gameStateManager, this));

	//
	/*m_window.create(sf::VideoMode(m_Config.resolution.x, m_Config.resolution.y),
		m_Config.windowName,Style::Fullscreen);*/
	m_window.create(VideoMode(m_Config.resolution.x, m_Config.resolution.y),
		m_Config.windowName, Style::Fullscreen);


	m_inputManager->setRenderWindow(&m_window);

	m_gameStateManager.setState("MenuState");
	
	return true;
}

void Game::update()
{
	// starts the clock
	static Clock clock;
	// restart takes time
	Time deltaTime = clock.restart();
	const float fDeltaTimeSeconds = deltaTime.asSeconds();

	// must be first call
	m_inputManager->update();

	m_gameStateManager.update(fDeltaTimeSeconds);
	m_debugDraw->Update(fDeltaTimeSeconds);
}

void Game::draw()
{
	m_window.clear();

	m_gameStateManager.draw();

	m_debugDraw->Draw(m_window);

	m_window.display();
}

void Game::shutdown() const
{
	m_soundManager->Clear();
}


