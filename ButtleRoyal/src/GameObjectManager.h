#pragma once

#include <string>
#include <unordered_map>

#include "GameObject.h"

#include "EventBus.h"

class GameObjectManager
{
public:
	typedef std::unordered_map<std::string, GameObject::Ptr>
		GameObjectMap;

	void Init();
	void Shutdown();

	void addGameObject(GameObject::Ptr gameObject);
	GameObject::Ptr getGameObject(const std::string& id) const;

	GameObjectMap& getGameObjects() { return m_gameObjects; }

private:

	 GameObjectMap m_gameObjects;

	 // maintain a list of listeners that the manager is subscribed to,
	 // so that he can unsubscribe
	 std::list<EventBus::ListenerId> m_listeners;
};