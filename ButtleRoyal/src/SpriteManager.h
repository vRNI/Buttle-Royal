#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "EventBus.h"

class IRenderComponent;

class SpriteManager
{
public:

	typedef std::unordered_map<std::string, std::shared_ptr<Texture>> TileSetMap;

	//
	SpriteManager(RenderWindow& renderWindow)
		: m_renderWindow(renderWindow)
	{}

	void Init();
	void Shutdown();

	/// Set camera component
	void setCamera(IRenderComponent* cameraComp) { m_camera = cameraComp; }
	IRenderComponent* getCamera() const { return m_camera; }

	/// Add component to the named layer. If the component is already in a layer
	/// it is removed from this layer and put in this layer.
	void addCompToLayer(const std::string& layer, IRenderComponent* comp);
	void removeComp(IRenderComponent* comp);

	/// Get current layer of the given component.
	std::string getLayer(IRenderComponent* comp);

	/// Set up the order of layers by providing their names in order
	/// from back to front.
	void setLayerOrder(const std::vector<std::string>& layerOrder)
	{
		m_layerOrder = layerOrder;
	}

	//
	void setTileSize(const Vector2i& tileSize) { m_tileSize = tileSize; }
	Vector2i getTileSize() const { return m_tileSize; }

	//
	void setTileSets(TileSetMap& tileSets)
	{
		m_tileSets = tileSets;
	}
	const TileSetMap& getTileSets() const { return m_tileSets; }

	//
	RenderWindow& getWindow() { return m_renderWindow; }

	//
	void Draw();

private:

	RenderWindow& m_renderWindow;

	Vector2i m_tileSize{};
	TileSetMap m_tileSets;

	IRenderComponent* m_camera = nullptr;

	std::map<std::string, std::list<IRenderComponent*>> m_layerToComp;
	std::map<IRenderComponent*, std::string> m_compToLayer;

	std::vector<std::string> m_layerOrder;

	// maintain a list of listeners that the manager is subscribed to,
	// so that he can unsubscribe
	std::list<EventBus::ListenerId> m_listeners;
};