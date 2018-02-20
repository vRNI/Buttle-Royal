#include "stdafx.h"

#include "Debug.h"

#include "GameObjectManager.h"
#include "GameObjectEvents.h"

void GameObjectManager::Init()
{
	// subscribe to creation events
	{
		EventBus::ListenerId id = EventBus::getInstance().AddListener(GameObjectCreateEvent::sk_EventType,
			[this](IEvent::Ptr event)
		{
			auto goCreateEvent = std::static_pointer_cast<GameObjectCreateEvent>(event);
			this->addGameObject(goCreateEvent->GetData());
		});
		m_listeners.push_back(id);
	}
}

void GameObjectManager::Shutdown()
{
	m_gameObjects.clear();

	// unsubscribe from events
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
		EventBus::getInstance().RemoveListener(*it);
	m_listeners.clear();
}

void
GameObjectManager::addGameObject(GameObject::Ptr gameObject)
{
	FF_ASSERT_MSG(m_gameObjects.find(gameObject->GetId()) == m_gameObjects.end(),
		"Game object with this id already exists " + gameObject->GetId());

	m_gameObjects[gameObject->GetId()] = gameObject;
}

GameObject::Ptr GameObjectManager::getGameObject(const std::string& id) const
{
	auto it = m_gameObjects.find(id);
	if (it == m_gameObjects.end())
	{
		FF_ERROR_MSG("Could not find gameobject with id " + id);
		return nullptr;
	}
	return it->second;
}
