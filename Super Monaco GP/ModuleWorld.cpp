#include "ModuleWorld.h"

#include "Road.h"
#include <vector>
#include "Utils.h"
#include "Player.h"
#include "Static.h"
#include "Segment.h"
#include "Animated.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "Background.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleStart.h"
#include "CameraFollow.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleAnimation.h"
#include "ModuleCollision.h"
#include "ModuleGameObject.h"
#include "AnimationContainer.h"

using namespace std;

ModuleWorld::ModuleWorld(GameEngine* gameEngine) :
	Module(gameEngine)
{
	textureRectRoad = TEXTURE_RECT_ROAD;
	viewportRectRoad = VIEWPORT_RECT_ROAD;

	textureRectRoadMirror = TEXTURE_RECT_ROAD_MIRROR;
	viewportRectRoadMirror = VIEWPORT_RECT_ROAD_MIRROR;

	pausePosition = PAUSE_POSITION;

	goMenuPosition = GO_MENU_POSITION;

	bestLapPosition = BEST_LAP_POSITION;

	currentLapTimePosition = CURRENT_LAP_TIME_POSITION;

	currentVelocityPosition = CURRENT_VELOCITY_POSITION;

	mirrorBorderTrapezoidTop = MIRROR_BORDER_TRAPEZOID_TOP;
	mirrorBorderTrapezoidLeft = MIRROR_BORDER_TRAPEZOID_LEFT;
	mirrorBorderTrapezoidRight = MIRROR_BORDER_TRAPEZOID_RIGHT;
	mirrorBorderTrapezoidBottom = MIRROR_BORDER_TRAPEZOID_BOTTOM;
}

ModuleWorld::~ModuleWorld()
{ }

const Road* ModuleWorld::getRoad() const
{
	return road;
}

const Camera* ModuleWorld::getCamera() const
{
	return camera;
}

const Camera* ModuleWorld::getCameraMirror() const
{
	return cameraMirror;
}

// Revisar
// SDL_Rect textureRectRoadMirror{ 0, (int)((1.0f - 0.24f) * WINDOW_HEIGHT), WINDOW_WIDTH, (int)(0.24f * WINDOW_HEIGHT) };
// SDL_Rect viewportRoadMirror{ (int)(0.15f * WINDOW_WIDTH), (int)(0.12f * WINDOW_HEIGHT), (int)(0.7f * WINDOW_WIDTH), (int)(0.17f * WINDOW_HEIGHT) };

bool ModuleWorld::setUp()
{
	getGameEngine()->getModuleGameObject()->load();

	road = new Road();

	// road->load("Resources/Configurations/Roads/Italy.json", getGameEngine()->getModuleJson(), getGameEngine()->getModuleTexture());
	road->load("Resources/Configurations/Roads/Portugal.json", getGameEngine()->getModuleJson(), getGameEngine()->getModuleTexture());

	// GameObjects ---

	const vector<RoadGameObjectDefinition*>* gameObjectDefinitions = road->getGameObjectDefinitions();

	gameObjects.reserve(gameObjectDefinitions->size() + 2);

	// Player

	player = (Player*)addGameObject(0, WorldPosition{ 0.0f, 0.0f, 0.0f });

	// Cars

	addGameObject(1, WorldPosition{ 5.5f, 0.0f, 2.5f });
	addGameObject(1, WorldPosition{ -5.5f, 0.0f, 2.5f });
	addGameObject(1, WorldPosition{ 0.0f, 0.0f, 30.0f });
	addGameObject(1, WorldPosition{ 0.0f, 0.0f, -10.0f });

	// Environment

	for(uint i = 0; i < (uint)gameObjectDefinitions->size(); ++i)
	{
		RoadGameObjectDefinition* gameObjectDefinition = (*gameObjectDefinitions)[i];
		addGameObject(gameObjectDefinition->id, gameObjectDefinition->wp, gameObjectDefinition->offsetX);
	}

	// --- GameObjects

	camera = new CameraFollow(true, road, player->getPosition());
	cameraMirror = new CameraFollow(false, road, player->getPosition(), 1.1f, 11.0f, WorldPosition{ 0.0f, 0.0f, -CAMERA_Y });

	// Background ---

	const RoadBackgroundDefinition* roadBackgroundDefinition = road->getRoadBackgroundDefinition();
	
	background = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), camera->getProjectionY1());
	backgroundMirror = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), cameraMirror->getProjectionY1());

	// --- Background

	layerRoad = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoad, &viewportRectRoad);
	layerRoadMirror = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoadMirror, &viewportRectRoadMirror);

	return true;
}

// #include <string>

bool ModuleWorld::update(float deltaTimeS)
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		paused = !paused;
		pauseCounter = 0.0f;
	}

	if(!paused)
	{
		for(uint i = 0; i < (uint)gameObjects.size(); ++i)
			gameObjects[i]->update(deltaTimeS);

		camera->update(deltaTimeS);
		cameraMirror->update(deltaTimeS);

		background->update((Player*)player, road, deltaTimeS);
		backgroundMirror->update((Player*)player, road, deltaTimeS);

		currentLapTime += deltaTimeS; //
	}
	else
	{
		updatePauseCounter(deltaTimeS);

		if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
			getGameEngine()->setGameModule(new ModuleStart(getGameEngine()));
	}

	getGameEngine()->getModuleRenderer()->setLayer(layerRoad);
	background->render(!camera->getForward(), getGameEngine()->getModuleRenderer());
	road->render(camera, getGameEngine()->getModuleRenderer());

	renderUI();

	getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	backgroundMirror->render(!cameraMirror->getForward(), getGameEngine()->getModuleRenderer());
	road->render(cameraMirror, getGameEngine()->getModuleRenderer());

	return true;
}

void ModuleWorld::cleanUp()
{
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoad);
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoadMirror);

	getGameEngine()->getModuleCollision()->removeColliders();

	if(road)
	{
		road->unload(getGameEngine()->getModuleTexture());
		delete road; road = nullptr;
	}

	if(camera)
	{
		delete camera; camera = nullptr;
	}

	if(cameraMirror)
	{
		delete cameraMirror; cameraMirror = nullptr;
	}

	player = nullptr;

	gameObjects.clear();

	delete background; background = nullptr;
	delete backgroundMirror; backgroundMirror = nullptr;

	getGameEngine()->getModuleGameObject()->unload();
}

void ModuleWorld::updatePauseCounter(float deltaTimeS)
{
	pauseCounter = mod0L(pauseCounter + deltaTimeS, 1.0f);
}

void ModuleWorld::renderUI() const
{
	// Mirror border

	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidTop, 0xFF202020);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidLeft, 0xFF202020);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidRight, 0xFF606060);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidBottom, 0xFF606060);

	// Best lap

	getGameEngine()->getModuleFont()->renderText("BEST LAP", bestLapPosition, HAlignment::LEFT, VAlignment::BOTTOM, BEST_LAP_POSITION_SCALE, BEST_LAP_POSITION_SCALE, 224, 160, 0);

	// Current velocity

	getGameEngine()->getModuleFont()->renderText(to_string(kmh(player->getVelocity())), currentVelocityPosition, HAlignment::RIGHT, VAlignment::TOP, CURRENT_VELOCITY_POSITION_SCALE, CURRENT_VELOCITY_POSITION_SCALE, 224, 160, 0);

	// Current lap time

	string currentLapTimeStr; time(currentLapTime, currentLapTimeStr);
	getGameEngine()->getModuleFont()->renderText(currentLapTimeStr, currentLapTimePosition, HAlignment::CENTER, VAlignment::BOTTOM, CURRENT_LAP_TIME_POSITION_SCALE, CURRENT_LAP_TIME_POSITION_SCALE, 224, 160, 0);

	// Pause

	if(paused)
	{
		if(pauseCounter <= 0.5f)
			getGameEngine()->getModuleFont()->renderText("P A U S E", pausePosition, HAlignment::CENTER, VAlignment::CENTER, PAUSE_POSITION_SCALE, PAUSE_POSITION_SCALE, 224, 160, 0);

		getGameEngine()->getModuleFont()->renderText("PRESS ESCAPE FOR MENU", goMenuPosition, HAlignment::CENTER, VAlignment::CENTER, GO_MENU_POSITION_SCALE, GO_MENU_POSITION_SCALE, 248, 252, 248);
	}
}

GameObject* ModuleWorld::addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad)
{
	GameObject* gameObject = getGameEngine()->getModuleGameObject()->getGameObject(id);

	// gameObject->setRoad(road);
	gameObject->setModuleWorld(this);
	gameObject->enableCollider();

	// Adjust game object's position

	WorldPosition wP = worldPosition;

	wP.x += xOffsetRoad * (ROAD_WIDTH / 2.0f);
	wP.z = road->getSegmentAtZ(wP.z)->getZNear() + SEGMENT_LENGTH / 2.0f;

	gameObject->setPosition(wP);
	gameObject->elevate();

	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->z)->addGameObject(gameObject);

	return gameObject;
}