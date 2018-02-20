#include "stdafx.h"

#include "MainState.h"
#include "Game.h"
#include "InputManager.h"

#include "CameraRenderComponent.h"
#include "DebugDraw.h"

#include "NLTmxMap.h"
#include "TileMapLoader.h"
#include "BoxColliderComponent.h"
#include "PhysicsComponentEvents.h"
#include "PlayerPropertyComponent.h"
#include "WinState.h"
#include "PlayerStateManager.h"
#include "SpriteAnimationComponent.h"
#include "Trigonometry.h"
#include "VectorAlgebra3D.inl"


using namespace std;
void MainState::VHandleEvent(Event event)
{
	m_gui.handleEvent(event);
}

MainState::MainState(GameStateManager* gameStateManager, Game* game)
	: GameState(gameStateManager, game)
	, m_spriteManager(game->getWindow())
	, m_timeToChangeGravity(0.f)
{}

void MainState::checkGround(shared_ptr<BoxColliderComponent> collComp)
{
	// Feedback: Derartig lange Codestücke sollten nicht mit einer Lambda implementiert werden sondern in eine Funktion ausgelagert werden (übersichtlicher).
	// Die Zeilen sind generell viel zu lang und das erschwert das Lesen ungemein! Statt zB ständig collider1.GetGameObject().getPosition()... aufzufufen 
	// ist es besser sich die Position vorher mal in eine Variable abzuspeichern. Wir programmieren für Menschen!

	collComp->registerOnCollisionFunction([&](BoxColliderComponent& collider1, BoxColliderComponent& collider2)
	{
		if (collider2.GetGameObject().GetType() == "Trigger")
		{
			resetPlayer(collider1.GetGameObject());
		}
		if (collider2.GetGameObject().GetType() == "Platform")
		{
			auto playerPosition = collider1.GetGameObject().getPosition();
			auto platformPosition = collider2.GetGameObject().getPosition();
			auto playerProperties = collider1.GetGameObject().GetComponent<PlayerPropertyComponent>();

			if (playerProperties->isNormalGravity())
			{
				if (platformPosition.y - playerPosition.y > collider1.getTransformedAABB().height)
				{
					playerProperties->setOnGround(true);
				}
			}
			else
			{
				if (playerPosition.y - platformPosition.y > collider2.getTransformedAABB().height + collider1.getAABB().height)
				{
					playerProperties->setOnGround(true);
				}
			}
		}
	});
}

// Feedback: Diese Methode ist viel zu lang (viele andere im Projekt auch).
// Kurze Methoden sind leichter zu verstehen und zu debuggen! Raw-Strings und
// Magic-Values bitte weitmöglichst vermeiden.
void MainState::VInit()
{
	SoundManager::getInstance().Clear();
	m_gameObjectManager.Init();
	m_spriteManager.Init();
	m_physicsManager.Init();
	
	
	if (!m_font.loadFromFile("../assets/Fonts/aerxtabs_memesbruh03.ttf"))
	{
		err() << "Could not load font\n";
		return;
	}
	const int mapIdx = PlayerPropertyManager::getInstance().getMap();
	{
		
		const string &resourcePath = "../assets/Maps/";
		NLTmxMap* tilemap = NLLoadTmxMap(resourcePath + "buttleRoyal_" + to_string(mapIdx) + ".tmx");
		FF_ASSERT_MSG(tilemap != nullptr, "Could not load tilemap " + resourcePath + "buttleRoyal_4.tmx");

		LoadTileLayers(tilemap, resourcePath, m_spriteManager);
		LoadObjectLayers(tilemap, resourcePath, m_spriteManager);

		delete tilemap;
	}

	Vector2f screenCenter{ static_cast<float>(m_game->getWindow().getSize().x) *.5f,static_cast<float>(m_game->getWindow().getSize().y) * .5f };
	m_gui.setWindow(m_game->getWindow());
	m_gui.setFont(m_font);
	m_theme = make_shared<tgui::Theme>("../assets/Black.txt");
	//auto button = make_shared<tgui::Button>();

	switch(mapIdx)
	{
	case 1: bg = make_shared<tgui::Picture>("../assets/Maps/background_blank.png");
		break;
	case 2: bg = make_shared<tgui::Picture>("../assets/Maps/background_day.png");
		break;
	case 3: bg = make_shared<tgui::Picture>("../assets/Maps/background_blank_night.png");
		break;
	case 4: bg = make_shared<tgui::Picture>("../assets/Maps/background_night.png");
		break;
	}


	bg->setPosition(0, 0);
	bg->moveToBack();
	m_gui.add(bg);

	//Player 1 GUI Properties
	{
		player1 = m_theme->load("Button");
		player1->setSize(250, 110);
		player1->setTextSize(32);
		player1->setPosition(50, 20);
		m_gui.add(player1, "Player1");
	}

	//Player 2 GUI Properties
	{
		player2 = m_theme->load("Button");
		player2->setSize(250, 110);
		player2->setTextSize(32);
		player2->setPosition(m_game->getWindow().getSize().x - 350, 20);
		m_gui.add(player2, "Player2");
	}

	gravityChange = m_theme->load("Button");

	gravityChange->setText("Gravity  \nChange! ");
	gravityChange->setSize(250, 130);
	gravityChange->setPosition(screenCenter.x - 220, 0);
	m_gui.add(gravityChange, "Gravity Change");
	m_changeSlider = m_theme->load("Slider");
	m_changeSlider->setMinimum(0);
	m_changeSlider->setMaximum(30);
	m_changeSlider->setPosition(screenCenter.x-200, 150);
	m_gui.add(m_changeSlider);
	
	// Moving camera
	{
		auto camera = make_shared<GameObject>("Camera");
		View v = m_game->getWindow().getView();
		auto renderComp = make_shared<CameraRenderComponent>(
			 *camera, m_game->getWindow(), v);
		camera->AddComponent(renderComp);
		camera->Init();

		m_gameObjectManager.addGameObject(camera);
		m_spriteManager.setCamera(renderComp.get());
	}

	{
		auto playerOne = m_gameObjectManager.getGameObject("PlayerOne");
		playerOne->GetComponent<PlayerPropertyComponent>()->changeGravity();
	}

	//Playerlogik

	auto collComp1 = m_gameObjectManager.getGameObject("PlayerOne")->GetComponent<BoxColliderComponent>();
	auto collComp2 = m_gameObjectManager.getGameObject("PlayerTwo")->GetComponent<BoxColliderComponent>();
	checkGround(collComp1);
	checkGround(collComp2);

	// Define layer order manually here. Could come from custom file settings.
	m_spriteManager.setLayerOrder({ "Floor", "Background", "GameObjects", "Top" });
}

Vector2f MainState::getOffsetVec(GameObject::Ptr player, const Vector2f offset)
{
	Vector2f vec = player->getPosition();
	if (player->GetComponent<PlayerPropertyComponent>()->isNormalGravity())
	{
		vec.x += offset.x;
		vec.y += offset.y;
	}
	else
	{
		vec.x += offset.x;
		vec.y -= offset.y;
	}
	return vec;
}

Vector2f MainState::getOffsetVec(Vector2f player, const Vector2f offset, bool inverse)
{
	if (!inverse)
	{
		player.x += offset.x;
		player.y += offset.y;
	}
	else
	{
		player.x += offset.x;
		player.y -= offset.y;
	}
	return player;
}

void MainState::resetPlayer(GameObject& player)
{
	auto playerGameObject = m_gameObjectManager.getGameObject(player.GetId());
	auto playerProperties = playerGameObject->GetComponent<PlayerPropertyComponent>();

	Vector2f originalPos = playerProperties->getStartPosition();
	playerGameObject->setPosition(originalPos.x, originalPos.y);
	playerProperties->takeLive();
	playerProperties->refreshDamage();

	SoundManager::getInstance().PlaySound("Death1");

	if(playerProperties->getLives() <= 0)
	{
		PlayerPropertyManager::getInstance().setWhoIsDead(player.GetId());
		m_gameStateManager->setState("WinState");
	}
}

void MainState::hitResponse(GameObject::Ptr enemy, Vector2f toEnemy)
{
	float length = sqrt(pow(toEnemy.x, 2) + pow(toEnemy.y, 2));
	Vector2f knockback = Vector2f (toEnemy.x/length, toEnemy.y/length); // normalize
	
	enemy->GetComponent<SpriteAnimationComponent>()->Fade();
	m_timeToFade = 1.5f;
	enemy->GetComponent<SpriteAnimationComponent>()->Fade();

	float damagescale = enemy->GetComponent<PlayerPropertyComponent>()->getDamage();
	knockback *= damagescale;		

	if(!DebugDraw::getInstance().IsEnabled())
		cout << "Enemy = " << enemy->GetId() << "	Vector to enemy is: ( " << knockback.x << " | " << knockback.y << " )" << endl;

	enemy->GetComponent<RigidBodyComponent>()->AddVelocity(knockback);
}

void MainState::hitDetection(Vector2f attackPosition, GameObject::Ptr enemy, const float attacksize, const float damage)
{
	Vector2f distance;
	const Vector2f enemyCenter = getOffsetVec(enemy, offsetToMiddle);
	distance.x = enemyCenter.x - attackPosition.x;
	distance.y = enemyCenter.y - attackPosition.y;
	const float length = distance.x * distance.x + distance.y * distance.y;
	const float distSqr = (m_playerhitcirclesize + attacksize) *  (m_playerhitcirclesize + attacksize);
	
	if (abs(length) < distSqr)
	{
		if(attacksize== m_airattacksize)
		{
			SoundManager::getInstance().PlaySound("hitResponse1");
		}
		else
			SoundManager::getInstance().PlaySound("hitResponse2");
		
		hitResponse(enemy, distance);
		enemy->GetComponent<PlayerPropertyComponent>()->addDamage(damage);
	}
}

// Feedback: Achtung, umbedingt bei der bennenung von Klassen und Methoden mehr Mühe geben!
// (es ist normal das ca. die hälfte der Programmierzeit nur für die Benennung drauf geht).
// Es ist nicht logisch das von einer Methode mit dem Namen "isHitAllowed" der rückgabewert garnicht benutzt wird
// sondern durch den aufruf zwei Schachtelungseben tiefer das Leben des Gegners abgezogen wird. 
// Eine Methode die so heißt soll nur zurückliefern ob ja oder nein und keine Logik ausführen!
// Mit const hättet ihr euch bereits bei der deffinition selber davor schützen können später diesen Fehler zu machen.

void MainState::checkForHit(GameObject::Ptr attacker, const GameObject::Ptr enemy)
{
	Vector2f coll1 = getOffsetVec(attacker, offsetToMiddle);
	const Vector2f coll2 = getOffsetVec(enemy, offsetToMiddle);
	const auto attackerProperties = attacker->GetComponent<PlayerPropertyComponent>();
	const auto attackerCurrentState = attacker->GetComponent<PlayerStateManager>()->getCurrentStateName();

	// Feedback: Ich habe es mir nicht im Detail durchgedacht aber diese Passage riecht dannach als gehöre
	// sie in den PlayerState. Das sehe ich an den if's und an den Funktionsaufrufen die sich je nach State nur minimal unterscheiden.
	if (attackerProperties->hasHit())
	{
		if (attackerCurrentState == "LeftPunchState")
		{
			coll1 = getOffsetVec(coll1, offsetLeftPunch, attackerProperties->isNormalGravity());
			hitDetection(coll1, enemy, m_groundattacksize, m_groundattackDamage);
		}

		if (attackerCurrentState == "RightPunchState")
		{
			coll1 = getOffsetVec(coll1, offsetRightPunch, attackerProperties->isNormalGravity());
			hitDetection(coll1, enemy, m_groundattacksize, m_groundattackDamage);
		}

		if (attackerCurrentState == "LeftAirPunchState")
		{
			coll1 = getOffsetVec(coll1, offsetLeftAir, attackerProperties->isNormalGravity());
			hitDetection(coll1, enemy, m_airattacksize, m_airattackDamage);
		}

		if (attackerCurrentState == "RightAirPunchState")
		{
			coll1 = getOffsetVec(coll1, offsetRightAir, attackerProperties->isNormalGravity());
			hitDetection(coll1, enemy, m_airattacksize, m_airattackDamage);
		}
	}
}

void MainState::VUpdate(float fDeltaTime)
{
	// Feedback: Zeilen kürzen durch Ref-Caching
	auto playerOne = m_gameObjectManager.getGameObject("PlayerOne");
	auto playerTwo = m_gameObjectManager.getGameObject("PlayerTwo");

	auto playerOneProperties = playerOne->GetComponent<PlayerPropertyComponent>();
	auto playerTwoProperties = playerTwo->GetComponent<PlayerPropertyComponent>();

	player1->setText("Player1\nLife: " + to_string(playerOneProperties->getLives()) + "\tDMG: " + to_string(playerOneProperties->getDamage() / 10));
	player2->setText("Player2\nLife: " + to_string(playerTwoProperties->getLives()) + "\tDMG: " + to_string(playerTwoProperties->getDamage() / 10));

	
	m_timeToChangeGravity += fDeltaTime;
	if(m_timeToChangeGravity > 30 )
	{
		m_timeToChangeGravity = 0;
		playerOneProperties->changeGravity();
		playerTwoProperties->changeGravity();
	}
	
	m_changeSlider->setValue(m_timeToChangeGravity);
	
	if (InputManager::getInstance().isKeyPressed("Exit") || InputManager::getInstance().isButtonPressed("Exit"))
	{
		m_gameStateManager->setState("MenuState");
		return;
	}

	{
		checkForHit(playerOne, playerTwo);
		checkForHit(playerTwo, playerOne);
	}

	m_timeToFade -= fDeltaTime;
	if (m_timeToFade < 0.f)
	{
		playerOne->GetComponent<SpriteAnimationComponent>()->FadeOut();
		playerTwo->GetComponent<SpriteAnimationComponent>()->FadeOut();
		m_timeToFade = 0;
	}

	m_physicsManager.Update(fDeltaTime);
	EventBus::getInstance().ProcessEvents(fDeltaTime);

	// update remaining game objects
	for (auto goPair : m_gameObjectManager.getGameObjects())
	{
		goPair.second->Update(fDeltaTime);
	}

}

void MainState::VDraw()
{
	m_gui.draw();
	m_spriteManager.Draw();


	//Debug Draw Enablen
	if(!DebugDraw::getInstance().IsEnabled())
	{
		for (auto obj : m_physicsManager.GetColliderComponents())
		{
			auto debug = new RectangleShape({ obj->getAABB().width,obj->getAABB().height });

			debug->setFillColor(Color::Transparent);
			debug->setOutlineColor(Color::Red);
			debug->setOutlineThickness(2);
			debug->setPosition(obj->getAABB().left, obj->getAABB().top);

			m_game->getWindow().draw(*debug, obj->GetGameObject().getTransform());
		}
	}
}

void MainState::VExit()
{
	m_physicsManager.Shutdown();
	m_spriteManager.Shutdown();
	m_gameObjectManager.Shutdown();
}