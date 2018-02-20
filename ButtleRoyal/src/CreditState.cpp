#include "stdafx.h"

#include "CreditState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"

using namespace std;


void CreditState::VHandleEvent(Event event)
{
	m_gui.handleEvent(event);
}

void
CreditState::VInit()
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

	Vector2f screenCenter{ static_cast<float>(m_game->getWindow().getSize().x) *.5f,static_cast<float>(m_game->getWindow().getSize().y) * .5f };
	m_gui.setWindow(m_game->getWindow());
	m_gui.setFont(m_font);

	auto bg = make_shared<tgui::Picture>("../assets/Maps/background_blank.png");

	bg->setPosition(0, 0);
	bg->moveToBack();
	m_gui.add(bg);

	m_theme = make_shared<tgui::Theme>("../assets/Black.txt");
	{
		auto vert = make_shared<tgui::VerticalLayout>();
		vert->setSize(500, 400);
		vert->setPosition(screenCenter.x - 250, screenCenter.y - 200);
		vert->moveToBack();

		PlayButton = m_theme->load("Button");
		PlayButton->setText("Arnold Holler");
		PlayButton->connect("Clicked", [&]()
		{
			m_gameStateManager->setState("MainState");
		});

		vert->add(PlayButton, "b1");
		vert->addSpace(0.2f);
		//PlayButton->connect("Clicked", &onClicked(), this);

		ControlsButton = m_theme->load("Button");
		ControlsButton->setText("Michael Etschbacer");
		vert->add(ControlsButton, "b2");
		vert->addSpace(0.2f);

		CreditsButton = m_theme->load("Button");
		CreditsButton->setText("Irma Muminovic");
		CreditsButton->setPosition(100, 50);
		vert->add(CreditsButton, "b3");
		vert->addSpace(0.2f);

		m_gui.add(vert);
	}

	m_view = m_game->getWindow().getView();

	m_isInit = true;
}



void CreditState::VUpdate(float delta)
{
	m_game->getWindow().setView(m_view);

	if (InputManager::getInstance().isKeyPressed("Exit") || InputManager::getInstance().isButtonDown("Exit"))
		m_gameStateManager->setState("MenuState");
}

void CreditState::VDraw()
{
	m_gui.draw();
	m_game->getWindow().draw(m_text);
}