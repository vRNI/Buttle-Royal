#include "stdafx.h"

#include "GameObject.h"
#include "TileLayerRenderComponent.h"

TileLayerRenderComponent::TileLayerRenderComponent(
	GameObject &gameObject, 
	RenderWindow &renderWindow,
	const TileLayer &layer)
: IRenderComponent(gameObject, renderWindow)
, m_layer(layer)
{
	
}

bool 
TileLayerRenderComponent::VInit()
{
	return true;
}

void 
TileLayerRenderComponent::VUpdate(float fDeltaTime)
{
	//
	// Unoptimized implementation of culling not visible tiles.
	// One possible optimization would be assigning tiles
	// to bounding regions and selecting regions based on their 
	// visibility. 

	m_visibleTiles.clear();

	View view = m_renderWindow.getView();

	const Vector2f& center = view.getCenter();
	const Vector2f& size = view.getSize();

	const float offsetNumTiles = 2.0f;
	const Vector2f offset = Vector2f(m_layer.tileSize) * offsetNumTiles;

	const Vector2f& topLeft = center - 0.5f * size - offset;
	const Vector2f& bottomRight = center + 0.5f * size + offset;

	const Vector2i numRowsCols{ 
		static_cast<int>((bottomRight.x - topLeft.x) / m_layer.tileSize.x),
		static_cast<int>((bottomRight.y - topLeft.y) / m_layer.tileSize.y) };

	// Calculation is the inverse of what happens in TileMapLoader,
	// when calculating the pixel position of the tile.
	auto pixelToIndex = [](const Vector2f& pixelCoordinate, 
		const Vector2i& tileSize, const Vector2i& dim) -> int
	{
		return static_cast<int>(pixelCoordinate.y / tileSize.y * dim.x + pixelCoordinate.x / tileSize.x);
	};
	auto compFunction = [](const Tile& a, const Tile& value) -> bool
	{
		return a.idx < value.idx;
	};

	const Vector2f rowLength{static_cast<float>(m_layer.dimension.x*m_layer.tileSize.x), 0.0f };
	for (int i = 0; i < numRowsCols.y; ++i)
	{
		Vector2f left = topLeft + static_cast<float>(i) * rowLength;
		const int tileIdx = pixelToIndex(left, m_layer.tileSize, m_layer.dimension);
		
		auto it = std::lower_bound(m_layer.tiles.begin(), m_layer.tiles.end(),
			Tile{ tileIdx }, compFunction);
		
		if(it == m_layer.tiles.end())
			break;

		while (it != m_layer.tiles.end() && it->idx < tileIdx + numRowsCols.x)
		{
			m_visibleTiles.push_back(*it);
			++it;
		}
	}
}

void
TileLayerRenderComponent::VDraw()
{
	for (auto& tile : m_visibleTiles)
		m_renderWindow.draw(*(tile.sprite));
}
