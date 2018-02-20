#pragma once
#include "AnimatedSprite.h"
#include "GameObject.h"
#include "IRenderComponent.h"


class SpriteAnimationComponent : public IRenderComponent
{
public:
	typedef std::shared_ptr < SpriteAnimationComponent > Ptr;

	enum State
	{
		STATE_IDLELEFT,
		STATE_IDLERIGHT,
		STATE_WALKRIGHT,
		STATE_WALKLEFT,
		STATE_PUNCHRIGHT,
		STATE_PUNCHLEFT,
		STATE_JUMPINGRIGHT,
		STATE_JUMPINGLEFT,
		STATE_AIRBORNRIGHT,
		STATE_AIRBORNLEFT,
		STATE_LANDINGRIGHT,
		STATE_LANDINGLEFT,
		STATE_AIRPUNCHRIGHT,
		STATE_AIRPUNCHLEFT
	};

	SpriteAnimationComponent(GameObject& gameObject,
		RenderWindow& renderWindow,
		const std::string& textureFile,
		const Color& maskColor = Color::Black);

	bool VInit() override;
	void VDraw() override;
	void VUpdate(float fDeltaTime) override;
	void setTexture(Texture &texture);
	//void KeyBoardStates();
	Animation ChangeAnimationTo(State state);
	bool GetIsFullyLooped() { return m_isFullyLooped; }
	void SetIsFullyLooped(bool loop) { m_isFullyLooped = loop; }
	void Fade();
	void FadeOut();

private:
	bool m_isFullyLooped = false;
	enum State state_;
	enum State laststate_;
	std::map<std::string, Animation> m_animations;
	Animation m_walkingAnimationLeft;
	Animation m_walkingAnimationRight;
	Animation m_standingPunchRight;
	Animation m_standingPunchLeft;
	Animation m_idleRight;
	Animation m_idleLeft;
	Animation m_jumpingLeft;
	Animation m_jumpingRight;
	Animation m_airbornLeft;
	Animation m_airbornRight;
	Animation m_airpunchLeft;
	Animation m_airpunchRight;
	Animation m_landingLeft;
	Animation m_landingRight;
	AnimatedSprite m_animated_sprite;
	Animation* m_currentAnimation;
	bool m_noKeyPressed;
	int m_rows;
	Clock frameClock;
	std::string m_textureFile;
	Texture m_texture;
	Color m_maskColor;
	float m_fadeTime = 0.1f;
};