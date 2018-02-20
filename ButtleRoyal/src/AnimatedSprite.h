////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef ANIMATEDSPRITE_INCLUDE
#define ANIMATEDSPRITE_INCLUDE

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Animation.h"
#include "stdafx.h"

class AnimatedSprite : public Drawable, public Transformable
{
public:
	explicit AnimatedSprite(Time frameTime = seconds(0.2f), bool paused = false, bool looped = true);

	void update(Time deltaTime);
	void setAnimation(const Animation& animation);
	void setFrameTime(Time time);
	void play();
	void play(const Animation& animation);
	void pause();
	void stop();
	void setLooped(bool looped);
	void setColor(const Color& color);
	const Animation* getAnimation() const;
	FloatRect getLocalBounds() const;
	FloatRect getGlobalBounds() const;
	bool isLooped() const;
	bool isPlaying() const;
	Time getFrameTime() const;
	void setFrame(size_t newFrame, bool resetTime = true);
	int getCurrentFrame() const { return m_currentFrame; }

	bool hasFullyLooped() const { return m_fullLoop; }
	void retFullyLoopedFalse() { m_fullLoop = false; }
private:
	const Animation* m_animation;
	Time m_frameTime;
	Time m_currentTime;
	size_t m_currentFrame;
	bool m_isPaused;
	bool m_isLooped;
	const Texture* m_texture;
	Vertex m_vertices[4];
	bool m_fullLoop = false;

	void draw(RenderTarget& target, RenderStates states) const override;

};

#endif // ANIMATEDSPRITE_INCLUDE