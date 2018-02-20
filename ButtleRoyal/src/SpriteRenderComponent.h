#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

class SpriteRenderComponent : public IRenderComponent
{
public:
	typedef std::shared_ptr<SpriteRenderComponent> Ptr;

	SpriteRenderComponent(GameObject &gameObject,
					RenderWindow &renderWindow,
					const std::string& textureFile);

	virtual bool VInit() override;
	virtual void VUpdate(float fDeltaTime) override {}
	virtual void VDraw() override;

	Sprite& getSprite() { return m_sprite;  }

	/// Move window over sprite texture
	//void setTextureRect(const IntRect &intRect);

private:
	std::string m_textureFile;
	Texture m_texture;
	Sprite m_sprite;
};