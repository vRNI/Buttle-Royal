#include "stdafx.h"

#include "SpriteManager.h"
#include "IRenderComponent.h"

#include "RenderComponentEvents.h"

void SpriteManager::Init()
{
	// subscribe to creation events
	{
		EventBus::ListenerId id = EventBus::getInstance().AddListener(RenderableCreateEvent::sk_EventType,
			[this](IEvent::Ptr event)
		{
			auto renderableCreateEvent = std::static_pointer_cast<RenderableCreateEvent>(event);
			this->addCompToLayer(renderableCreateEvent->GetLayer(), &renderableCreateEvent->GetData());
		});
		m_listeners.push_back(id);
	}
}

void SpriteManager::Shutdown()
{
	m_layerOrder.clear();
	m_layerToComp.clear();
	m_compToLayer.clear();
	m_tileSets.clear();
	m_camera = nullptr;

	// unsubscribe from events
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
		EventBus::getInstance().RemoveListener(*it);
	m_listeners.clear();
}

void
SpriteManager::addCompToLayer(const std::string& layer, IRenderComponent* comp)
{
	if (m_compToLayer.find(comp) != m_compToLayer.end())
	{
		FF_ERROR_MSG("Removing component from layer, before adding to new layer. from " 
			+ m_compToLayer[comp] + " to " + layer);
		
		m_layerToComp[layer].remove(comp);
	}
	
	m_compToLayer[comp] = layer;
	m_layerToComp[layer].push_back(comp);
}

void 
SpriteManager::removeComp(IRenderComponent* comp)
{
	auto it = m_compToLayer.find(comp);
	if (it == m_compToLayer.end())
		return;

	m_layerToComp[it->second].remove(comp);
	m_compToLayer.erase(it);
}

std::string 
SpriteManager::getLayer(IRenderComponent* comp)
{
	auto it = m_compToLayer.find(comp);
	if (it != m_compToLayer.end())
		return it->second;
	return{};
}

void
SpriteManager::Draw()
{
	FF_ASSERT_MSG(m_camera != nullptr, "Camera is nullptr");

	m_camera->VDraw();

	for (auto order : m_layerOrder)
	{
		FF_ASSERT_MSG(m_layerToComp.find(order) != m_layerToComp.end(),
			"Layer with this name not defined: " + order);

		auto& layer = m_layerToComp[order];
		for (auto comp : layer)
			comp->VDraw();
	}
}
