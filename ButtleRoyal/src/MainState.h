#pragma once

#include "GameState.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "PhysicsManager.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/TGUI.hpp>

class FINALFRONTIER_API MainState : public GameState
{
public:
	MainState(GameStateManager* gameStateManager, Game* game);

	void checkGround(std::shared_ptr<BoxColliderComponent> collComp);

	virtual void VInit() override;
	virtual void VExit() override;
	virtual void VUpdate(float delta) override;
	virtual void VDraw() override;
	virtual void VHandleEvent(Event event) override;

	/// \brief resets Playert to his startposition
	void resetPlayer(GameObject& player);



	 /// \brief callculates position from gameobject to a certain offset
	/// \param player is given gameobject
	/// \param offset is given vector
	/// \return 
	static Vector2f getOffsetVec(GameObject::Ptr player, Vector2f offset);
	
	///\brief callculates position from vector to a certain offset
	///\param player is given original vector
	///\param offset is given vector
	///\param inverse deceides if the vector will be inverted
	///\return 
	Vector2f getOffsetVec(Vector2f player, Vector2f offset, bool inverse);

	///\brief 
	///\param enemy is gameobject that will receive response
	///\param toEnemy is from attackPosition to Enemy for knockback
	void hitResponse(GameObject::Ptr enemy, Vector2f toEnemy);

	///\brief checks for collision of specific punch with the Enemysposition
	///\param attackPosition is the centerpoint of the attack
	///\param enemy is gameobject that collision is checked with
	///\param attacksize changes the collisiondistance
	///\param damage that enemy will receive
	void hitDetection(Vector2f attackPosition, GameObject::Ptr enemy, float attacksize, float damage);

	///\brief checks if attack is pressed and which attack will be executed
	///\param player1 is activating punch
	///\param player2 is give so we can detect hit later
	void checkForHit(GameObject::Ptr player1, GameObject::Ptr player2);

private:
	SpriteManager m_spriteManager;
	GameObjectManager m_gameObjectManager;
	PhysicsManager m_physicsManager;

	std::shared_ptr<tgui::Button> player1;
	std::shared_ptr<tgui::Button> player2;

	std::shared_ptr< tgui::Picture > bg;
	std::shared_ptr<tgui::Button> gravityChange;
	std::shared_ptr<tgui::Label> m_player1Info;
	std::shared_ptr<tgui::Slider> m_changeSlider;
	Text m_text;
	Font m_font;
	View m_view;
	tgui::Gui m_gui;
	std::shared_ptr<tgui::Theme> m_theme;
	Clock m_clock;
	float m_timeToChangeGravity;
	float m_timeToFade = 0;
	

	//attackstats
	int m_playerhitcirclesize = 75;
	int m_groundattacksize = 40;
	int m_airattacksize = 60;
	int m_groundattackDamage = 150;
	int m_airattackDamage = 250;
	Vector2f offsetToMiddle = Vector2f(100, 100);
	Vector2f offsetRightPunch = Vector2f(60, -24);
	Vector2f offsetLeftPunch = Vector2f(-60, -24);
	Vector2f offsetRightAir = Vector2f(66, 40);
	Vector2f offsetLeftAir = Vector2f(-66, 40);
};
