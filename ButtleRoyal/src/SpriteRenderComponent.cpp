#include "stdafx.h"

#include "GameObject.h"
#include "SpriteRenderComponent.h"

SpriteRenderComponent::SpriteRenderComponent(
	GameObject &gameObject, 
	RenderWindow &renderWindow,
	const std::string& textureFile)
: IRenderComponent(gameObject, renderWindow)
, m_textureFile(textureFile)
{
	
}

bool 
SpriteRenderComponent::VInit()
{
	Image image;
	if(!image.loadFromFile(m_textureFile))	
	{
		err() << "Could not load texture from " << m_textureFile << std::endl;
		return false;
	}
	image.createMaskFromColor(Color::Black);
	m_texture.loadFromImage(image);
	m_sprite.setTexture(m_texture);
		
	return true;
}

void
SpriteRenderComponent::VDraw()
{
	// TODO: adapt trasnform to have pixel precise movement

	m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
}
