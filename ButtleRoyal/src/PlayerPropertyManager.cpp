#include "stdafx.h"

#include "PlayerPropertyManager.h"

PlayerPropertyManager& PlayerPropertyManager::getInstance()
{
	static PlayerPropertyManager instance;
	return instance;
}

void PlayerPropertyManager::Refresh()
{
	m_whoIsDead = "";
}



