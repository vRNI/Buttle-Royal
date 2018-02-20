#include "stdafx.h"

#include "ControlsState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"

using namespace std;


void ControlsState::VHandleEvent(const Event event)
{
	m_gui.handleEvent(event);
}

void
ControlsState::VInit()
{
	SoundManager::getInstance().Clear();
	if (m_isInit)
		return;

	if (!m_font.loadFromFile("../assets/Fonts/ArcadeAlternate.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}

	//Vector2f screenCenter{ static_cast<float>(m_game->getWindow().getSize().x) *.5f,static_cast<float>(m_game->getWindow().getSize().y) * .5f };
	m_gui.setWindow(m_game->getWindow());

	m_gui.setFont(m_font);

	auto bg = make_shared<tgui::Picture>("../assets/controls.png");
	bg->setPosition(0, 0);
	bg->moveToBack();
	m_gui.add(bg);
	m_theme = make_shared<tgui::Theme>("../assets/Black.txt");

	m_view = m_game->getWindow().getView();

	m_isInit = true;
}



void ControlsState::VUpdate(float delta)
{
	m_game->getWindow().setView(m_view);

	if (InputManager::getInstance().isKeyPressed("Exit") || InputManager::getInstance().isButtonPressed("Exit"))
		m_gameStateManager->setState("MenuState");
}

void ControlsState::VDraw()
{
	m_gui.draw();
	m_game->getWindow().draw(m_text);
}