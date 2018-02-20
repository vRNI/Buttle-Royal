#pragma once

#include "IComponent.h"


class PlayerPropertyComponent : public IComponent
{
public:
	typedef std::shared_ptr<PlayerPropertyComponent> Ptr;


	 /// \brief this component is for safing properties of the players
	 /// \param gameObject is owning object
	 /// \param numberOfLives 
	 /// \param sPos is the resetposition of the player
	PlayerPropertyComponent(GameObject &gameObject, int numberOfLives, Vector2f sPos);

	virtual bool VInit() override { return true; }
	virtual void VUpdate(float fDeltaTime) override;

	void addDamage(const int damage) { m_takenDamage += damage; }
	int getDamage() const { return  m_takenDamage; };
	void refreshDamage() { m_takenDamage = 0; }

	void takeLive() { m_lives--; }
	int getLives() const { return  m_lives; }

	Vector2f getStartPosition() const { return m_startPosition; }

	void setOnGround(const bool value) { m_onGround = value; }
	bool isOnGround() const { return m_onGround; }

	void setHasJumped(const bool value) { m_hasJumped = value; }
	bool hasJumped() const { return m_hasJumped; }

	void setHasDoubleJumped(const bool value) { m_hasDoubleJumped = value; }
	bool hasDoubleJumped() const { return m_hasDoubleJumped; }


	bool hasHit() const { return m_hasHit; }

	bool isNormalGravity() const { return m_NormalGravity; }
	void changeGravity();

private:
	int m_takenDamage;
	bool m_NormalGravity;
	int m_lives;
	Vector2f m_startPosition;
	bool m_onGround;
	bool m_hasJumped;
	bool m_hasDoubleJumped;
	bool m_hasHit;
};