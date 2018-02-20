#pragma once

#include <string>

#include "SpriteManager.h"
#include "GameObjectManager.h"

class NLTmxMap;

void LoadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager);

void LoadObjectLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager);
