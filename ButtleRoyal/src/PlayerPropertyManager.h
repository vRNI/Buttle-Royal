#pragma once

#include <unordered_map>

#include "GameObject.h"

class PlayerPropertyManager
{
public:
	typedef std::unordered_map<std::string, GameObject::Ptr>
		GameObjectMap;

	static PlayerPropertyManager& getInstance();
	void setWhoIsDead(const std::string PlayerID) { m_whoIsDead = PlayerID; }
	std::string WhoIsDead() const { return m_whoIsDead; }
	void Refresh();

	int getMap() const { return m_mapIdx; }
	void setMap(const int mapIdx) { m_mapIdx=mapIdx; }

private:
	PlayerPropertyManager() = default;
	~PlayerPropertyManager() = default;

	PlayerPropertyManager(const PlayerPropertyManager& rhv) = delete;
	PlayerPropertyManager& operator= (const PlayerPropertyManager& rhv) = delete;
	int m_mapIdx = 4;
	std::string m_whoIsDead;
};
