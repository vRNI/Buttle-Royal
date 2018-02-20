#include "stdafx.h"

#include "WinState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Loading/Theme.hpp>
#include <TGUI/Widgets/Button.hpp>

using namespace std;
void WinState::VHandleEvent(Event event)
{
	
}

void
WinState::VInit()
{
	m_loser = PlayerPropertyManager::getInstance().WhoIsDead();
	PlayerPropertyManager::getInstance().Refresh();
	if (m_isInit)
		return;
	SoundManager::getInstance().Clear();
	if (!m_font.loadFromFile("../assets/Fonts/aerxtabs_memesbruh03.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}

	const Vector2f screenCenter{ static_cast<float>(m_game->getWindow().getSize().x) *.5f,static_cast<float>(m_game->getWindow().getSize().y) * .5f };
	m_gui.setWindow(m_game->getWindow());
	m_gui.setFont(m_font);

	auto bg = make_shared<tgui::Picture>("../assets/Maps/background_blank.png");

	bg->setPosition(0, 0);
	bg->moveToBack();
	m_gui.add(bg);

	//auto centerPicture = make_shared<tgui::Picture>("../assets/fliesen_blau.png");
	//centerPicture->setPosition(screenCenter.x - 600, screenCenter.y - 400);
	//centerPicture->setSize(1200, 800);

	//m_gui.add(centerPicture);

	auto mmtLogo = make_shared<tgui::Picture>("../assets/FHSalzburgMMT.png");
	mmtLogo->setPosition(screenCenter.x + 350, screenCenter.y + 280);
	mmtLogo->setSize(113, 71);

	m_gui.add(mmtLogo);

	auto winner = make_shared<tgui::Button>();
	winner->setText(m_loser + " has lost!");
	winner->setPosition(screenCenter.x - 300, screenCenter.y);
	winner->setSize(600, 300);
	m_gui.add(winner);
	m_view = m_game->getWindow().getView();

	m_isInit = true;
}

void WinState::VUpdate(float delta)
{
	m_game->getWindow().setView(m_view);

	if (InputManager::getInstance().isKeyPressed("Select") || InputManager::getInstance().isButtonPressed("Select"))
		m_gameStateManager->setState("MainState");
}

void WinState::VDraw()
{
	m_game->getWindow().draw(m_text);
	m_gui.draw();
}