#include "stdafx.h"

#include "MenuState.h"

#include "Game.h"
#include "GameStateManager.h"

#include "InputManager.h"

using namespace std;


void MenuState::VHandleEvent(Event event)
{
	m_gui.handleEvent(event);
}

void
MenuState::VInit()
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

	//auto bg = make_shared<tgui::Picture>("../assets/fliesen_gruen.png");
	//bg->moveToBack();
	//bg->setSize(m_game->getWindow().getSize().x, m_game->getWindow().getSize().y);
	//m_gui.add(bg);
	m_theme = make_shared<tgui::Theme>("../assets/Black.txt");
	m_theme2 = make_shared<tgui::Theme>("../assets/Black2.txt");
	m_theme3 = make_shared<tgui::Theme>("../assets/Black3.txt");

	//auto centerPicture = make_shared<tgui::Picture>("../assets/fliesen_blau.png");
	//centerPicture->setPosition(screenCenter.x - 600, screenCenter.y - 400);
	//centerPicture->setSize(1200, 800);

	//m_gui.add(centerPicture);

	auto mmtLogo = make_shared<tgui::Picture>("../assets/FHSalzburgMMT.png");
	mmtLogo->setPosition(screenCenter.x + 380, screenCenter.y + 90);
	mmtLogo->setSize(226,142);

	m_gui.add(mmtLogo);

	{
		const int posY = screenCenter.y-190;
		const int posX = screenCenter.x-600;

		auto playersVert = make_shared<tgui::VerticalLayout>();
		playersVert->setSize(100, 200);
		playersVert->setPosition(posX, posY);
		playersVert->moveToBack();

		playerOne = m_theme3->load("Button");
		playersVert->add(playerOne);

		playerTwo = m_theme2->load("Button");
		playersVert->add(playerTwo);
		m_gui.add(playersVert);

		auto playerNames = make_shared<tgui::VerticalLayout>();
		playerNames->setSize(200, 200);
		playerNames->setPosition(posX + 100, posY);

		auto playerOneName = make_shared<tgui::Button>();
		playerOneName->setText("Player One");
		playerNames->add(playerOneName);

		auto playerTwoName = make_shared<tgui::Button>();
		playerTwoName->setText("Player Two");
		playerNames->add(playerTwoName);
		playerNames->moveToBack();
		m_gui.add(playerNames);


	}

	{
		auto vert = make_shared<tgui::VerticalLayout>();
		vert->setSize(500, 400);
		vert->setPosition(screenCenter.x - 250, screenCenter.y - 200);
		vert->moveToBack();

		PlayButton = m_theme->load("Button");
		PlayButton->setText("Play");
		PlayButton->connect("Clicked",[&]()
		{
			PlayerPropertyManager::getInstance().setMap(4);
			m_gameStateManager->setState("MainState");
			SoundManager::getInstance().PlaySound("button"); 
		});
		//PlayButton->setTextSize(75);
		vert->add(PlayButton, "b1");
		vert->addSpace(0.2f);
		//PlayButton->connect("Clicked", &onClicked(), this);

		ControlsButton = m_theme->load("Button");
		ControlsButton->setText("Controls");
		ControlsButton->connect("Clicked", [&]()
		{
			m_gameStateManager->setState("ControlsState");
			SoundManager::getInstance().PlaySound("button");
		});
		vert->add(ControlsButton, "b2");
		vert->addSpace(0.2f);

		CreditsButton = m_theme->load("Button");
		CreditsButton->setText("Credits");
		CreditsButton->setPosition(100, 50);
		CreditsButton->connect("Clicked", [&]()
		{
			m_gameStateManager->setState("CreditState");
			SoundManager::getInstance().PlaySound("button");
		});
		vert->add(CreditsButton, "b3");
		vert->addSpace(0.2f);

		CreditsButton = m_theme->load("Button");
		CreditsButton->setText("Levels");
		CreditsButton->setPosition(100, 50);
		CreditsButton->connect("Clicked", [&]()
		{
			m_gameStateManager->setState("LevelState");
			SoundManager::getInstance().PlaySound("button");
		});
		vert->add(CreditsButton, "b4");
		vert->addSpace(0.2f);

		m_gui.add(vert);
	}

	m_view = m_game->getWindow().getView();

	m_isInit = true;
}



void MenuState::VUpdate(float delta)
{
	m_game->getWindow().setView(m_view);

	if (InputManager::getInstance().isKeyPressed("Select") || InputManager::getInstance().isButtonPressed("Select"))
		m_gameStateManager->setState("MainState");
}

void MenuState::VDraw()
{
	m_gui.draw();
	m_game->getWindow().draw(m_text);
}