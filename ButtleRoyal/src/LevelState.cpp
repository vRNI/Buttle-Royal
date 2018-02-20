#include "stdafx.h"

#include "MenuState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"
#include "LevelState.h"

using namespace std;


void LevelState::VHandleEvent(Event event)
{
	m_gui.handleEvent(event);
}

void
LevelState::VInit()
{
	SoundManager::getInstance().Clear();
	if (m_isInit)
		return;

	if (!m_font.loadFromFile("../assets/Fonts/aerxtabs_memesbruh03.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}
	
	m_text.setPosition(m_game->getWindow().getView().getCenter());
	m_text.setFillColor(Color::White);
	m_text.setFont(m_font);
	m_text.setOrigin(
		m_text.getLocalBounds().width * 0.5f,
		m_text.getLocalBounds().height * 0.5f);

	auto bg = make_shared<tgui::Picture>("../assets/Maps/background_blank.png");

	bg->setPosition(0, 0);
	bg->moveToBack();
	m_gui.add(bg);

	Vector2f screenCenter{ static_cast<float>(m_game->getWindow().getSize().x) *.5f,static_cast<float>(m_game->getWindow().getSize().y) * .5f };
	m_gui.setWindow(m_game->getWindow());
	
	m_gui.setFont(m_font);

	m_theme = make_shared<tgui::Theme>("../assets/Black.txt");
	m_theme2 = make_shared<tgui::Theme>("../assets/Black2.txt");
	m_theme3 = make_shared<tgui::Theme>("../assets/Black3.txt");

	auto mmtLogo = make_shared<tgui::Picture>("../assets/FHSalzburgMMT.png");
	mmtLogo->setPosition(screenCenter.x + 350, screenCenter.y + 280);
	mmtLogo->setSize(113, 71);

	m_gui.add(mmtLogo);

	{
		auto hor = make_shared<tgui::HorizontalLayout>();
		hor->setSize(1600, 300);
		hor->setPosition(200, 250);
		hor->moveToBack();
		const Color color{ 255, 255, 255 };
		hor->setBackgroundColor(color);

		map1 = make_shared<tgui::Picture>("../assets/Map1.png");
		map1->connect("Clicked", [&]()
		{
			PlayerPropertyManager::getInstance().setMap(1);
			m_gameStateManager->setState("MainState");
			SoundManager::getInstance().PlaySound("button");

		});
		hor->add(map1, "b1");
		hor->addSpace(0.2f);

		map2 = make_shared<tgui::Picture>("../assets/Map2.png");
		map2->connect("Clicked", [&]()
		{
			PlayerPropertyManager::getInstance().setMap(2);
			m_gameStateManager->setState("MainState");
			SoundManager::getInstance().PlaySound("button");
		});
		hor->add(map2, "b2");
		hor->addSpace(0.2f);

		map3 = make_shared<tgui::Picture>("../assets/Map3.png");
		map3->connect("Clicked", [&]()
		{
			PlayerPropertyManager::getInstance().setMap(3);
			m_gameStateManager->setState("MainState");
			SoundManager::getInstance().PlaySound("button");
		});
		hor->add(map3, "b3");
		hor->addSpace(0.2f);

		map4 = make_shared<tgui::Picture>("../assets/Map4.png");
		map4->connect("Clicked", [&]()
		{
			PlayerPropertyManager::getInstance().setMap(4);
			m_gameStateManager->setState("MainState");
			SoundManager::getInstance().PlaySound("button");
		});
		hor->add(map4, "b4");
		hor->addSpace(0.2f);

		m_gui.add(hor);
	}



	m_view = m_game->getWindow().getView();

	m_isInit = true;
}



void LevelState::VUpdate(float delta)
{
	m_game->getWindow().setView(m_view);

	if (InputManager::getInstance().isKeyPressed("Select") || InputManager::getInstance().isButtonPressed("Select"))
		m_gameStateManager->setState("MainState");
}

void LevelState::VDraw()
{
	m_gui.draw();
	m_game->getWindow().draw(m_text);
}