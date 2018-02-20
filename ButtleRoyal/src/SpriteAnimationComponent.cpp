#include "stdafx.h"
#include "SpriteAnimationComponent.h"
#include "InputManager.h"
#include "PlayerMoveComponent.h"

SpriteAnimationComponent::SpriteAnimationComponent(
	GameObject& gameObject,
	RenderWindow& renderWindow,
	const std::string& textureFile,
	const Color& maskColor)
	: IRenderComponent(gameObject, renderWindow)
	, m_currentAnimation(nullptr), m_noKeyPressed(false), m_rows(0), m_textureFile(textureFile)
	, m_maskColor(maskColor)
{
}

void setAnimation(Animation& animation, Texture& texture, const int row, const int col, const int coloffset)
{
	// spritewidth == spritelength
	const int spritewidth = 200;
	const int spriteheight = spritewidth;

	animation.setSpriteSheet(texture);
	int yOffset = row * spriteheight;
	int xOffset = coloffset* spritewidth;

	for (int i = 0; i < col; i++)
	{
		animation.addFrame(IntRect(xOffset, yOffset, 200, 200));
		xOffset += spritewidth;
	}
}

bool SpriteAnimationComponent::VInit()
{
	Image image;
	if (!image.loadFromFile(m_textureFile))
	{
		err() << "Could not load texture from " << m_textureFile << std::endl;
		return false;
	}
	image.createMaskFromColor(m_maskColor);
	m_texture.loadFromImage(image);

	setAnimation(m_walkingAnimationRight, m_texture, 0, 8, 0);
	setAnimation(m_walkingAnimationLeft, m_texture, 1, 8, 0);
	setAnimation(m_standingPunchRight, m_texture, 2, 5, 0);
	setAnimation(m_standingPunchLeft, m_texture, 3, 5, 0);
	setAnimation(m_jumpingRight, m_texture, 4, 2, 2); // UNNÖTIG ?
	setAnimation(m_jumpingLeft, m_texture, 5, 2, 2); // UNNÖTIG ?
	setAnimation(m_airbornRight, m_texture, 4, 1, 3);
	setAnimation(m_airbornLeft, m_texture, 5, 1, 3);
	setAnimation(m_landingRight, m_texture, 4, 1, 4);
	setAnimation(m_landingLeft, m_texture, 5, 1, 4);
	setAnimation(m_idleRight, m_texture, 6, 4, 0);
	setAnimation(m_idleLeft, m_texture, 7, 4, 0);
	setAnimation(m_airpunchRight, m_texture, 8, 3, 0);
	setAnimation(m_airpunchLeft, m_texture, 9, 3, 0);
	state_ = STATE_IDLELEFT;
	laststate_ = STATE_IDLELEFT;
	m_currentAnimation = &m_idleLeft;
	m_animated_sprite = AnimatedSprite(seconds(0.1), true, false);
	return true;
}

void SpriteAnimationComponent::setTexture(Texture &texture)
{
	m_texture = texture;
}

void SpriteAnimationComponent::VDraw()
{
	Transform t = m_gameObject.getTransform();
	//std::cout << m_gameObject.getPosition().x << " "<< m_gameObject.getPosition().y << std::endl;

	m_renderWindow.draw(m_animated_sprite, t);
}
Animation SpriteAnimationComponent::ChangeAnimationTo(State state)
{
	switch(state)
	{
	case STATE_IDLERIGHT:
		state_ = STATE_IDLERIGHT;
		m_noKeyPressed = false;
		return m_idleRight;
	case STATE_IDLELEFT:
		state_ = STATE_IDLELEFT;
		m_noKeyPressed = false;
		return m_idleLeft;
	case STATE_WALKRIGHT:
		state_ = STATE_WALKRIGHT;
		m_noKeyPressed = false;
		return m_walkingAnimationRight;
	case STATE_WALKLEFT:
		state_ = STATE_WALKLEFT;
		m_noKeyPressed = false;
		return m_walkingAnimationLeft;
	case STATE_PUNCHRIGHT:
		state_ = STATE_PUNCHRIGHT;
		m_noKeyPressed = false;
		return m_standingPunchRight;
	case STATE_PUNCHLEFT:
		state_ = STATE_PUNCHLEFT;
		m_noKeyPressed = false;
		return m_standingPunchLeft;
	case STATE_JUMPINGLEFT:
		state_ = STATE_JUMPINGLEFT;
		m_noKeyPressed = false;
		return m_jumpingLeft;
	case STATE_JUMPINGRIGHT:
		state_ = STATE_JUMPINGRIGHT;
		m_noKeyPressed = false;
		return m_jumpingRight;
	case STATE_AIRBORNLEFT:
		state_ = STATE_AIRBORNLEFT;
		m_noKeyPressed = false;
		return m_airbornLeft;
	case STATE_AIRBORNRIGHT:
		state_ = STATE_AIRBORNRIGHT;
		m_noKeyPressed = false;
		return m_airbornRight;
	case STATE_LANDINGLEFT:
		state_ = STATE_LANDINGLEFT;
		m_noKeyPressed = false;
		return m_landingLeft;
	case STATE_LANDINGRIGHT:
		state_ = STATE_LANDINGRIGHT;
		m_noKeyPressed = false;
		return m_landingRight;
	case STATE_AIRPUNCHRIGHT:
		state_ = STATE_AIRPUNCHRIGHT;
		m_noKeyPressed = false;
		return m_airpunchRight;
	case STATE_AIRPUNCHLEFT:
		state_ = STATE_AIRPUNCHLEFT;
		m_noKeyPressed = false;
		return m_airpunchLeft;
	default:
		break;
	}
}

//void SpriteAnimationComponent::KeyBoardStates()
//{
//
//}

void SpriteAnimationComponent::VUpdate(float fDeltaTime)
{
	Time frameTime = frameClock.restart();

	auto m_temp = ChangeAnimationTo(state_);

	if(&m_temp!=m_currentAnimation)
		m_currentAnimation = &m_temp;

	m_animated_sprite.play(*m_currentAnimation);

	//TODO test if the animated sprite is fully looped

	if (m_animated_sprite.hasFullyLooped()
		&& (state_ == STATE_PUNCHLEFT || state_ == STATE_PUNCHRIGHT) 
		|| (state_ == STATE_AIRPUNCHLEFT || state_ == STATE_AIRPUNCHRIGHT))
	{
		m_animated_sprite.stop();
		m_animated_sprite.retFullyLoopedFalse();
		m_noKeyPressed = true;
		m_isFullyLooped = true;
	}
	
	if (m_noKeyPressed) {
		m_animated_sprite.stop();
	}
	
	
	m_noKeyPressed = true;
	m_animated_sprite.update(frameTime);

}
void SpriteAnimationComponent::Fade()
{

	m_animated_sprite.setColor(Color::Red);
	
}
void SpriteAnimationComponent::FadeOut()
{
	m_animated_sprite.setColor(Color::White);
}
