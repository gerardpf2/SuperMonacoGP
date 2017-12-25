#include "ModuleWorld.h"

#include "Road.h"
#include "Utils.h"
#include "Player.h"
#include "Segment.h"
#include "Background.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleStart.h"
#include "CameraFollow.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleCollision.h"
#include "ModuleGameObject.h"

using namespace std;

ModuleWorld::ModuleWorld(GameEngine* gameEngine) :
	Module(gameEngine)
{
	textureRectRoad = TEXTURE_RECT_ROAD;
	viewportRectRoad = VIEWPORT_RECT_ROAD;

	textureRectRoadMirror = TEXTURE_RECT_ROAD_MIRROR;
	viewportRectRoadMirror = VIEWPORT_RECT_ROAD_MIRROR;

	kmhRect = KMH_RECT;

	pausePosition = PAUSE_POSITION;

	goMenuPosition = GO_MENU_POSITION;

	bestLapPosition = BEST_LAP_POSITION;

	bestLapValuePosition = BEST_LAP_VALUE_POSITION;

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

bool ModuleWorld::setUp()
{
	addRoad();

	addAllGameObjects();

	addCameras();

	addBackgrounds();

	addRenderingLayers();

	kmhT = getGameEngine()->getModuleTexture()->get(2, 16);

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	checkPauseMode();

	if(!paused) updateNotPaused(deltaTimeS);
	else updatePaused(deltaTimeS);

	render();

	return true;
}

void ModuleWorld::cleanUp()
{
	kmhT = nullptr;

	removeRenderingLayers();

	removeBackgrounds();

	removeCameras();

	removeAllGameObjects();

	removeRoad();
}

void ModuleWorld::registerLapTime(const Car* car) const
{ }

void ModuleWorld::addPlayer()
{
	player = (Player*)addGameObject(0, WorldPosition{ 0.0f, 0.0f, -5.0f });
}

void ModuleWorld::addCars()
{ }

void ModuleWorld::renderUI() const
{
	// Mirror border

	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidTop, 0xFF202020);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidLeft, 0xFF202020);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidRight, 0xFF606060);
	getGameEngine()->getModuleRenderer()->renderTrapezoid(mirrorBorderTrapezoidBottom, 0xFF606060);

	// Best lap

	getGameEngine()->getModuleFont()->renderText("BEST LAP", bestLapPosition, HAlignment::LEFT, VAlignment::BOTTOM, BEST_LAP_POSITION_SCALE, BEST_LAP_POSITION_SCALE, 224, 160, 0);
	
	string bestLapTimeStr; time(getGameEngine()->getModuleRegistry()->getPlayerBestLapTime(), bestLapTimeStr);
	getGameEngine()->getModuleFont()->renderText(bestLapTimeStr, bestLapValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, BEST_LAP_VALUE_POSITION_SCALE, BEST_LAP_VALUE_POSITION_SCALE, 248, 252, 248);

	// Current velocity

	getGameEngine()->getModuleFont()->renderText(to_string(kmh(player->getVelocity())), currentVelocityPosition, HAlignment::RIGHT, VAlignment::TOP, CURRENT_VELOCITY_POSITION_SCALE, CURRENT_VELOCITY_POSITION_SCALE, 224, 160, 0);
	getGameEngine()->getModuleRenderer()->renderTexture(kmhT->t, kmhT->r, &kmhRect, kmhT->hFlipped);

	// Current lap time

	string currentLapTimeStr; time(player->getCurrentLapTime(), currentLapTimeStr);
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

void ModuleWorld::addRoad()
{
	const char* roadJsonFile;

	if(getGameEngine()->getModuleRegistry()->getCurrentCourseId() == 0)
		roadJsonFile = "Resources/Configurations/Roads/Italy.json";
	else
		roadJsonFile = "Resources/Configurations/Roads/Portugal.json";

	road = new Road();
	road->load(roadJsonFile, getGameEngine()->getModuleJson(), getGameEngine()->getModuleTexture());
}

void ModuleWorld::removeRoad()
{
	if(road)
	{
		road->unload(getGameEngine()->getModuleTexture());
		delete road; road = nullptr;
	}
}

void ModuleWorld::addRoadGameObjects()
{
	const vector<RoadGameObjectDefinition*>* gameObjectDefinitions = road->getGameObjectDefinitions();

	for(uint i = 0; i < (uint)gameObjectDefinitions->size(); ++i)
	{
		RoadGameObjectDefinition* gameObjectDefinition = (*gameObjectDefinitions)[i];
		addGameObject(gameObjectDefinition->id, gameObjectDefinition->wp, gameObjectDefinition->offsetX);
	}
}

void ModuleWorld::addAllGameObjects()
{
	getGameEngine()->getModuleGameObject()->load();

	gameObjects.reserve(road->getGameObjectsCount() + 1);

	addPlayer();

	addCars();

	// At this point all added game objects are cars (including player)
	// Assign an id to each one

	for(uint i = 0; i < (uint)gameObjects.size(); ++i)
		((Car*)gameObjects[i])->setSpecificId(i);

	addRoadGameObjects();
}

void ModuleWorld::removeAllGameObjects()
{
	getGameEngine()->getModuleCollision()->removeColliders();

	player = nullptr;

	gameObjects.clear();

	getGameEngine()->getModuleGameObject()->unload();
}

void ModuleWorld::addCameras()
{
	camera = new CameraFollow(true, road, player->getPosition());
	cameraMirror = new CameraFollow(false, road, player->getPosition(), 1.1f, 11.0f, WorldPosition{ 0.0f, 0.0f, -CAMERA_Y });
}

void ModuleWorld::removeCameras()
{
	if(camera)
	{
		delete camera; camera = nullptr;
	}

	if(cameraMirror)
	{
		delete cameraMirror; cameraMirror = nullptr;
	}
}

void ModuleWorld::addBackgrounds()
{
	const RoadBackgroundDefinition* roadBackgroundDefinition = road->getRoadBackgroundDefinition();

	background = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), camera->getProjectionY1());
	backgroundMirror = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), cameraMirror->getProjectionY1());
}

void ModuleWorld::removeBackgrounds()
{
	delete background; background = nullptr;
	delete backgroundMirror; backgroundMirror = nullptr;
}

void ModuleWorld::addRenderingLayers()
{
	layerRoad = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoad, &viewportRectRoad);
	layerRoadMirror = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoadMirror, &viewportRectRoadMirror);
}

void ModuleWorld::removeRenderingLayers()
{
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoad);
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoadMirror);

	layerRoad = layerRoadMirror = -1;
}

void ModuleWorld::checkPauseMode()
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		paused = !paused;
		pauseCounter = 0.0f;
	}
}

void ModuleWorld::checkGoMenu() const
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		getGameEngine()->setGameModule(new ModuleStart(getGameEngine()));
}

void ModuleWorld::updatePaused(float deltaTimeS)
{
	checkGoMenu();

	updatePauseCounter(deltaTimeS);
}

void ModuleWorld::updateNotPaused(float deltaTimeS) const
{
	for(GameObject* gameObject : gameObjects)
		gameObject->update(deltaTimeS);

	camera->update(deltaTimeS);
	cameraMirror->update(deltaTimeS);

	background->update(player, road, deltaTimeS);
	backgroundMirror->update(player, road, deltaTimeS);
}

void ModuleWorld::updatePauseCounter(float deltaTimeS)
{
	pauseCounter = mod0L(pauseCounter + deltaTimeS, 1.0f);
}

void ModuleWorld::render() const
{
	getGameEngine()->getModuleRenderer()->setLayer(layerRoad);
	background->render(!camera->getForward(), getGameEngine()->getModuleRenderer());
	road->render(camera, getGameEngine()->getModuleRenderer());

	renderUI();

	getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	backgroundMirror->render(!cameraMirror->getForward(), getGameEngine()->getModuleRenderer());
	road->render(cameraMirror, getGameEngine()->getModuleRenderer());
}