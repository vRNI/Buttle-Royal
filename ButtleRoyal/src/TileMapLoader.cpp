#include "stdafx.h"

#include "TileMapLoader.h"
#include "NLTmxMap.h"
#include "TileLayerRenderComponent.h"
#include "PlayerMoveComponent.h"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.h"

#include "RenderComponentEvents.h"
#include "GameObjectEvents.h"
#include "SpriteAnimationComponent.h"
#include "MainState.h"
#include "PlayerPropertyComponent.h"
#include "PlayerStateManager.h"
#include "RightStandingState.h"
#include "LeftRunningState.h"
#include "RightRunningState.h"
#include "LeftStandingState.h"
#include "LeftPunchState.h"
#include "RightPunchState.h"
#include "LeftJumpingState.h"
#include "LeftAirbornState.h"
#include "LeftLandingState.h"
#include "RightLandingState.h"
#include "RightAirbornState.h"
#include "RightJumpingState.h"
#include "LeftAirPunchState.h"
#include "RightAirPunchState.h"

void LoadTileLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	err() << "Load tilemap with size: " << tilemap->width << ", "
		<< tilemap->height << " and tilesize: " << tilemap->tileWidth
		<< ", " << tilemap->tileHeight << std::endl;

	spriteManager.setTileSize({ tilemap->tileWidth, tilemap->tileHeight });
	//m_tileMap.dimension.x = tilemap->width;
	//m_tileMap.dimension.y = tilemap->height;

	// load textures for every tileset
	SpriteManager::TileSetMap tileSets;
	for (auto tileset : tilemap->tilesets)
	{
		err() << "Load tileset: " << tileset->name << " width filename: "
			<< tileset->filename << " and tilesize: " << tileset->tileWidth
			<< ", " << tileset->tileHeight << std::endl;

		auto texture = make_shared<Texture>();
		if (!texture->loadFromFile(resourcePath + tileset->filename))
			err() << "Could not load texture " << resourcePath + tileset->filename << endl;
		tileSets[tileset->name] = texture;
	}
	spriteManager.setTileSets(tileSets);

	// go through all layers
	std::vector<TileLayer> tileLayers;
	tileLayers.resize(tilemap->layers.size());

	for (int layerIdx = 0; layerIdx < static_cast<int>(tilemap->layers.size()); layerIdx++)
	{
		NLTmxMapLayer* layer = tilemap->layers[layerIdx];
		err() << "Load layer: " << layer->name << " with width: "
			<< layer->width << " and height: " << layer->height << std::endl;

		int size = layer->width * layer->height;

		tileLayers[layerIdx] = TileLayer{ layer->name, Vector2i{layer->width, layer->height},
			Vector2i{tilemap->tileWidth, tilemap->tileHeight }	};
			
		// go over all elements in the layer
		for (int i = 0; i < size; i++)
		{
			int grid = layer->data[i];

			if (grid == 0)
			{
				// 0 means there is no tile at this grid position.

				// This continue also indicates that the layers are not 
				// represented as two-dimensional arrays, but 
				// only a list of tiles sorted by position from top left
				// to bottom right. (Hint: position could be useful for 
				// quickly finding, which tiles to cull from current 
				// viewport.)					
				continue;
			}

			// get tileset and tileset texture
			NLTmxMapTileset* tileset = tilemap->getTilesetForGrid(grid);
			Vector2i tileSize(tilemap->tileWidth, tilemap->tileHeight);
			auto texture = tileSets[tileset->name];

			assert(texture != nullptr);

			// horizontal tile count in tileset texture
			int tileCountX = texture->getSize().x / tileSize.x;

			// calcualte position of tile
			Vector2f position;
			position.x = (i % layer->width) * static_cast<float>(tileSize.x);
			position.y = (i / layer->width) * static_cast<float>(tileSize.y);
			//position += offset;

			// calculate 2d idx of tile in tileset texture
			int idx = grid - tileset->firstGid;
			int idxX = idx % tileCountX;
			int idxY = idx / tileCountX;

			// calculate source area of tile in tileset texture
			IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);

			// create tile and put it into a layer
			auto sprite = make_shared<Sprite>();
			sprite->setTexture(*texture);
			sprite->setTextureRect(source);
			sprite->setPosition(position.x, position.y);

			tileLayers[layerIdx].tiles.push_back({ i, sprite });
		}

	
	}




	// Add objects to layers
	auto go = make_shared<GameObject>("TileMap");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(go));
	for (auto layer : tileLayers)
	{
		auto layerComp = make_shared<TileLayerRenderComponent>(
			*go, spriteManager.getWindow(), layer);
		go->AddComponent(layerComp);	

		EventBus::getInstance().FireEvent(std::make_shared<RenderableCreateEvent>(layer.name, *layerComp));
	}
	go->Init();
}

static GameObject::Ptr loadSprite(NLTmxMapObject* object, const std::string& layer,
	const std::string& resourcePath, SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	gameObject->SetType("Player");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->setPosition(static_cast<float>(object->x), static_cast<float>(object->y));


	auto playerStateManager = make_shared<PlayerStateManager>(*gameObject);
	gameObject->AddComponent(playerStateManager);
	
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftStandingState", make_shared<LeftStandingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftRunningState", make_shared<LeftRunningState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftPunchState", make_shared<LeftPunchState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftJumpingState", make_shared<LeftJumpingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftAirbornState", make_shared<LeftAirbornState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftLandingState", make_shared<LeftLandingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("LeftAirPunchState", make_shared<LeftAirPunchState>(playerStateManager));

	gameObject->GetComponent<PlayerStateManager>()->registerState("RightStandingState", make_shared<RightStandingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightRunningState", make_shared<RightRunningState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightPunchState", make_shared<RightPunchState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightJumpingState", make_shared<RightJumpingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightAirbornState", make_shared<RightAirbornState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightLandingState", make_shared<RightLandingState>(playerStateManager));
	gameObject->GetComponent<PlayerStateManager>()->registerState("RightAirPunchState", make_shared<RightAirPunchState>(playerStateManager));

	
	//set first State
	gameObject->GetComponent<PlayerStateManager>()->setState("RightStandingState");
	
	
	
	// TODO: the following code would be a good candiate for a factory
	// that creates the components based on the input properties.

	// Parse data from file
	/*IntRect textureRect{};
	textureRect.width = object->width;
	textureRect.height = object->height;*/
	std::string spriteTexture;
	int borderSize;
	bool input = false;
	int playerIdx = 0;
	float mass = 1.0f;
	for (auto property : object->properties)
	{
		auto name = property->name;		
		if (name == "Texture")
		{
			spriteTexture = resourcePath + property->value;
		}
		//else if (name == "BorderSize")
		//{
		//	borderSize = std::stoi(property->value);
		//}
		//else if (name == "TextureRectLeft")
		//{
		//	textureRect.left = stoi(property->value);
		//}
		//else if (name == "TextureRectTop")
		//{
		//	textureRect.top = stoi(property->value);
		//}
		else if (name == "InputPlayerIdx")
		{
			input = true;
			playerIdx = stoi(property->value);
		}
		else if (name == "Mass")
		{
			mass = stof(property->value);
		}
	}

	// Initialize components with parsed data.
	if (spriteTexture.length() > 0)
	{
		auto  renderComp = make_shared<SpriteAnimationComponent>(
			*gameObject, spriteManager.getWindow(), spriteTexture
			);		
		gameObject->AddComponent(renderComp);

		//renderComp->getSprite().setTextureRect(textureRect);
		//renderComp->getSprite().setOrigin(textureRect.width * 0.5f, textureRect.height *0.5f);
		//renderComp->getSprite().setPosition(0.0f, 0.0f);

		EventBus::getInstance().FireEvent(std::make_shared<RenderableCreateEvent>(layer, *renderComp));
	}

	int numberOfLives = 4;
	auto propertycomp = make_shared<PlayerPropertyComponent>(*gameObject, numberOfLives, Vector2f(static_cast<float>(object->x), static_cast<float>(object->y)));
	propertycomp->setOnGround(false); // error otherwise
	gameObject->AddComponent(propertycomp);	
	auto rigidComp = make_shared<RigidBodyComponent>(*gameObject, mass);
	gameObject->AddComponent(rigidComp);
	auto collComp = make_shared<BoxColliderComponent>(*gameObject, *rigidComp,
		FloatRect(46, 22, static_cast<float>(object->width)-92, static_cast<float>(object->height)-44),false);
	gameObject->AddComponent(collComp);

	collComp->registerOnCollisionFunction([&](BoxColliderComponent& collider1, BoxColliderComponent& collider2)
	{
		//cout << "Object-> " << collider1.GetGameObject().GetId() << " collides with Object-> " << collider2.GetGameObject().GetType() << endl;
		if(collider2.GetGameObject().GetType() == "Trigger" )
		{
			//EventBus::getInstance().QueueEvent(std::make_shared<BoxColliderCreateEvent>(*collComp));
		}
	});

	if (input)
	{
		auto inputComp =
			make_shared<PlayerMoveComponent>(*gameObject, *rigidComp, playerIdx);
		gameObject->AddComponent(inputComp);
	}

	gameObject->Init();
	return gameObject;
}

static GameObject::Ptr loadBackground(NLTmxMapObject* object, const std::string& layer,
	const std::string& resourcePath, SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	gameObject->SetType("Background");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

	gameObject->setPosition(static_cast<float>(object->x), static_cast<float>(object->y));

	// TODO: the following code would be a good candiate for a factory
	// that creates the components based on the input properties.

	// Parse data from file
	/*IntRect textureRect{};
	textureRect.width = object->width;
	textureRect.height = object->height;*/
	std::string spriteTexture;
	int borderSize = 0;
	bool input = false;
	//int playerIdx = 0;
	//float mass = 1.0f;
	for (auto property : object->properties)
	{
		auto name = property->name;
		if (name == "Texture")
		{
			spriteTexture = resourcePath + property->value;
		}
	}

	// Initialize components with parsed data.
	if (spriteTexture.length() > 0)
	{
		auto  renderComp = make_shared<SpriteAnimationComponent>(
			*gameObject, spriteManager.getWindow(), spriteTexture
			);
		gameObject->AddComponent(renderComp);

		//renderComp->getSprite().setTextureRect(textureRect);
		//renderComp->getSprite().setOrigin(textureRect.width * 0.5f, textureRect.height *0.5f);
		//renderComp->getSprite().setPosition(0.0f, 0.0f);

		EventBus::getInstance().FireEvent(std::make_shared<RenderableCreateEvent>("Floor", *renderComp));
	}

	gameObject->Init();
	return gameObject;
}

static GameObject::Ptr loadCollider(NLTmxMapObject* object, const std::string& layer,
	const std::string& resourcePath, SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	gameObject->SetType("Collider");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));
	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// TODO: the following code would be a good candiate for a factory
	// that creates the components based on the input properties.

	auto rigidComp = make_shared<RigidBodyComponent>(*gameObject, 0.0f);
	gameObject->AddComponent(rigidComp);

	auto collComp = make_shared<BoxColliderComponent>(*gameObject, *rigidComp,
		FloatRect(0, 0, static_cast<float>(object->width), static_cast<float>(object->height)),false);
	gameObject->AddComponent(collComp);
	
	gameObject->Init();
	return gameObject;
}

static GameObject::Ptr loadPlatform(NLTmxMapObject* object, const std::string& layer,
	const std::string& resourcePath, SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	gameObject->SetType("Platform");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));
	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// TODO: the following code would be a good candiate for a factory
	// that creates the components based on the input properties.

	auto rigidComp = make_shared<RigidBodyComponent>(*gameObject, 0.0f);
	gameObject->AddComponent(rigidComp);

	auto collComp = make_shared<BoxColliderComponent>(*gameObject, *rigidComp,
		FloatRect(0, 0, static_cast<float>(object->width), static_cast<float>(object->height)), false);
	gameObject->AddComponent(collComp);

	gameObject->Init();
	return gameObject;
}

static GameObject::Ptr loadTrigger(NLTmxMapObject* object, const std::string& layer,
	const std::string& resourcePath, SpriteManager& spriteManager)
{
	auto gameObject = make_shared<GameObject>(object->name);
	gameObject->SetType("Trigger");
	EventBus::getInstance().FireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));
	gameObject->move(static_cast<float>(object->x), static_cast<float>(object->y));

	// TODO: the following code would be a good candiate for a factory
	// that creates the components based on the input properties.

	auto rigidComp = make_shared<RigidBodyComponent>(*gameObject, 0.0f);
	gameObject->AddComponent(rigidComp);

	auto collComp = make_shared<BoxColliderComponent>(*gameObject, *rigidComp,
		FloatRect(0, 0, static_cast<float>(object->width), static_cast<float>(object->height)),true);
	gameObject->AddComponent(collComp);

	gameObject->Init();
	return gameObject;
}

void LoadObjectLayers(NLTmxMap* tilemap, const std::string& resourcePath,
	SpriteManager& spriteManager)
{
	// go through all object layers
	for (auto group : tilemap->groups)
	{
		// go over all objects per layer
		for (auto object : group->objects)
		{
			Vector2f position(static_cast<float>(object->x), static_cast<float>(object->y));
			//position += offset;

			//FloatRect bounds(position.x, position.y, (float) (object->width), (float) (object->height));

			// TODO  the following code would be a good candiate for a factory
			// that creates the components based on the input properties.

			if (object->type == "Sprite")
				auto sprite = loadSprite(object, group->name, resourcePath, spriteManager);
			if (object->type == "Collider")
				auto collider = loadCollider(object, group->name, resourcePath, spriteManager);
			if (object->type == "Trigger")
				auto collider = loadTrigger(object, group->name, resourcePath, spriteManager);
			if (object->type == "Platform")
				auto collider = loadPlatform(object, group->name, resourcePath, spriteManager);
			//if (object->type == "Attack")
			//	auto collider = loadAttack(object, group->name, resourcePath, spriteManager);
			if (object->type == "Background")
				auto collider = loadBackground(object, group->name, resourcePath, spriteManager);
		}
	}
}