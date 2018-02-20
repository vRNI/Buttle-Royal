#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

struct Tile
{
	// original tile index
	int idx;
	std::shared_ptr<sf::Sprite> sprite;
};

struct TileLayer
{
	std::string name;
	sf::Vector2i dimension;
	sf::Vector2i tileSize;
	std::vector<Tile> tiles;
};

class TileLayerRenderComponent : public IRenderComponent
{
public:
	typedef std::shared_ptr<TileLayerRenderComponent> Ptr;

	TileLayerRenderComponent(GameObject &gameObject,
							 RenderWindow &renderWindow,
							 const TileLayer& layer);

	virtual bool VInit() override;
	virtual void VUpdate(float fDeltaTime) override; 
	virtual void VDraw() override;

	const TileLayer& getLayer() { return m_layer; }

private:
	
	TileLayer m_layer;

	std::list<Tile> m_visibleTiles;
};