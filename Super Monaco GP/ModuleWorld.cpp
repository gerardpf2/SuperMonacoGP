#include "ModuleWorld.h"

#include "Road.h"
#include "Utils.h"
#include "Player.h"
#include "Segment.h"
#include "Minimap.h"
#include "Animation.h"
#include "Background.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSwitch.h"
#include "CameraFollow.h"
#include "ModuleTexture.h"
#include "ModuleWorldUI.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"
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

	currentLapTimeNotificationPosition = CURRENT_LAP_TIME_NOTIFICATION_POSITION;

	currentLapTimeNotificationValuePosition = CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION;

	currentVelocityPosition = CURRENT_VELOCITY_POSITION;

	mirrorBorderTrapezoidTop = MIRROR_BORDER_TRAPEZOID_TOP;
	mirrorBorderTrapezoidLeft = MIRROR_BORDER_TRAPEZOID_LEFT;
	mirrorBorderTrapezoidRight = MIRROR_BORDER_TRAPEZOID_RIGHT;
	mirrorBorderTrapezoidBottom = MIRROR_BORDER_TRAPEZOID_BOTTOM;

	lightRect = LIGHT_RECT;
}

ModuleWorld::~ModuleWorld()
{ }

const Road* ModuleWorld::getRoad() const
{
	return road;
}

const Player* ModuleWorld::getPlayer() const
{
	return player;
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
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleTexture());
	assert(getGameEngine()->getModuleRenderer());
	assert(getGameEngine()->getModuleAnimation());

	addRoad();

	addMinimap();

	addAllGameObjects();

	addCameras();

	addBackgrounds();

	addRenderingLayers();

	audioGroupId = getGameEngine()->getModuleAudio()->load("Resources/Configurations/Audios/World.json");

	kmhT = getGameEngine()->getModuleTexture()->get(2, 16);

	lightIdAnimationGroup = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/Light.json");
	lightAnimation = getGameEngine()->getModuleAnimation()->getAnimation(lightIdAnimationGroup, 0);

	// When the setUp function is executed, this module is still blocked
	// At this point, this is rendered without being updated
	// All the cameras need to be positioned first
	// Then, it is necessary to compute the segment offsets by rendering the road
	// Finally, all the game objects (especially the cars) need to be updated in order to compute the correct texture that is going to be rendered
	// The cars compute that based on the difference between their position and the position of the camera
	// If all the previous steps are ignored all is still working, but the initial rendered scene is not correct
	// Once the module is not blocked any more, that scene is correctly rendered
	// By doing the previous steps it is possible for the player to see all being correctly rendered while the module is still blocked

	assert(camera);
	assert(cameraMirror);

	camera->update(0.0f);
	cameraMirror->update(0.0f);

	assert(road);

	road->render(camera, getGameEngine()->getModuleRenderer());

	for(GameObject* gameObject : gameObjects)
	{
		assert(gameObject);

		gameObject->update(0.0f);
	}

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		checkPauseMode();

		checkRenderCollisionBoxes();

		if(!paused) updateNotPaused(deltaTimeS);
		else updatePaused(deltaTimeS);
	}

	render();

	return true;
}

void ModuleWorld::cleanUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleAnimation());

	getGameEngine()->getModuleAudio()->unload(audioGroupId);

	getGameEngine()->getModuleAnimation()->unload(lightIdAnimationGroup);
	
	lightAnimation = nullptr;

	kmhT = nullptr;

	removeRenderingLayers();

	removeBackgrounds();

	removeCameras();

	removeAllGameObjects();

	removeMinimap();

	removeRoad();
}

void ModuleWorld::registerLapTime(const Car* car)
{
	assert(car);

	if(car->getType() == GameObjectType::PLAYER)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleAudio());

		currentLapTimeNotificate = true;
		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 3, 1.0f, 4);
	}
}

void ModuleWorld::playFxOutRoad()
{
	if(!outRoadFxPlayed)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleAudio());

		outRoadFxPlayed = true;
		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 2);
	}
}

void ModuleWorld::playFxCollision() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());

	getGameEngine()->getModuleAudio()->playFx(audioGroupId, 4);
}

void ModuleWorld::addPlayer()
{
	player = (Player*)addGameObject(0, WorldPosition{ 0.0f, 0.0f, -SEGMENT_LENGTH });
}

void ModuleWorld::addCars()
{ }

void ModuleWorld::renderUI() const
{
	assert(kmhT);
	assert(player);
	assert(lightAnimation);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRegistry());
	
	ModuleFont* moduleFont = getGameEngine()->getModuleFont();
	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	assert(moduleFont);
	assert(moduleRenderer);

	// Mirror border

	moduleRenderer->renderTrapezoid(mirrorBorderTrapezoidTop, 0xFF202020);
	moduleRenderer->renderTrapezoid(mirrorBorderTrapezoidLeft, 0xFF202020);
	moduleRenderer->renderTrapezoid(mirrorBorderTrapezoidRight, 0xFF606060);
	moduleRenderer->renderTrapezoid(mirrorBorderTrapezoidBottom, 0xFF606060);

	// Best lap

	moduleFont->renderText("BEST LAP", bestLapPosition, HAlignment::LEFT, VAlignment::BOTTOM, BEST_LAP_POSITION_SCALE, BEST_LAP_POSITION_SCALE, 224, 160, 0);
	
	string bestLapTimeStr; time(getGameEngine()->getModuleRegistry()->getPlayerBestLapTime(), bestLapTimeStr);
	moduleFont->renderText(bestLapTimeStr, bestLapValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, BEST_LAP_VALUE_POSITION_SCALE, BEST_LAP_VALUE_POSITION_SCALE, 248, 252, 248);

	// Current velocity

	moduleFont->renderText(to_string(kmh(player->getVelocity())), currentVelocityPosition, HAlignment::RIGHT, VAlignment::TOP, CURRENT_VELOCITY_POSITION_SCALE, CURRENT_VELOCITY_POSITION_SCALE, 224, 160, 0);
	moduleRenderer->renderTexture(kmhT->t, kmhT->r, &kmhRect, kmhT->hFlipped);

	// Current lap time

	if(renderCurrentLapTime)
	{
		string currentLapTimeStr; time(player->getCurrentLapTime(), currentLapTimeStr);
		moduleFont->renderText(currentLapTimeStr, currentLapTimePosition, HAlignment::CENTER, VAlignment::BOTTOM, CURRENT_LAP_TIME_POSITION_SCALE, CURRENT_LAP_TIME_POSITION_SCALE, 224, 160, 0);
	}

	// Current lap time notification

	if(currentLapTimeNotificate)
	{
		if(roundf(currentLapTimeNotificationCounter) == (int)currentLapTimeNotificationCounter)
		{
			moduleFont->renderText("LAP TIME", currentLapTimeNotificationPosition, HAlignment::RIGHT, VAlignment::CENTER, CURRENT_LAP_TIME_NOTIFICATION_POSITION_SCALE, CURRENT_LAP_TIME_NOTIFICATION_POSITION_SCALE, 248, 252, 248);

			string currentLapTimeStoredStr; time(player->getCurrentLapTimeStored(), currentLapTimeStoredStr);
			moduleFont->renderText(currentLapTimeStoredStr, currentLapTimeNotificationValuePosition, HAlignment::LEFT, VAlignment::CENTER, CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_SCALE, CURRENT_LAP_TIME_NOTIFICATION_VALUE_POSITION_SCALE, 224, 160, 0);
		}
	}

	// Light

	if(!lightAnimation->hasEnded())
	{
		const Texture* lightT = lightAnimation->getCurrentFrame();

		assert(lightT);

		moduleRenderer->renderTexture(lightT->t, lightT->r, &lightRect, lightT->hFlipped);
	}

	// Pause

	if(paused)
	{
		if(pauseCounter <= 0.5f)
			moduleFont->renderText("P A U S E", pausePosition, HAlignment::CENTER, VAlignment::CENTER, PAUSE_POSITION_SCALE, PAUSE_POSITION_SCALE, 224, 160, 0);

		moduleFont->renderText("PRESS ESCAPE FOR MENU", goMenuPosition, HAlignment::CENTER, VAlignment::CENTER, GO_MENU_POSITION_SCALE, GO_MENU_POSITION_SCALE, 248, 252, 248);
	}
}

GameObject* ModuleWorld::addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad)
{
	assert(road);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleGameObject());

	GameObject* gameObject = getGameEngine()->getModuleGameObject()->getGameObject(id);

	assert(gameObject);

	gameObject->setModuleWorld(this);

	// Adjust game object's position

	WorldPosition wP = worldPosition;

	wP.x += xOffsetRoad * (ROAD_WIDTH / 2.0f);

	Segment* segment = road->getSegmentAtZ(wP.z);

	assert(segment);

	wP.z = segment->getZNear() + SEGMENT_LENGTH / 2.0f;

	gameObject->setPosition(wP);
	gameObject->elevate();

	gameObjects.push_back(gameObject);

	segment->addGameObject(gameObject);

	return gameObject;
}

void ModuleWorld::addRoad()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRegistry());

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
		assert(getGameEngine());
		assert(getGameEngine()->getModuleTexture());

		road->unload(getGameEngine()->getModuleTexture());
		
		delete road; road = nullptr;
	}
}

void ModuleWorld::addMinimap()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRegistry());

	const char* minimapJsonFile;

	if(getGameEngine()->getModuleRegistry()->getCurrentCourseId() == 0)
		minimapJsonFile = "Resources/Configurations/Minimaps/Italy.json";
	else
		minimapJsonFile = "Resources/Configurations/Minimaps/Portugal.json";

	minimap = new Minimap(road);
	minimap->load(minimapJsonFile, getGameEngine()->getModuleJson(), getGameEngine()->getModuleTexture());
}

void ModuleWorld::removeMinimap()
{
	if(minimap)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleTexture());

		minimap->unload(getGameEngine()->getModuleTexture());
		
		delete minimap; minimap = nullptr;
	}
}

void ModuleWorld::addRoadGameObjects()
{
	assert(road);
	assert(road->getGameObjectDefinitions());

	for(RoadGameObjectDefinition* roadGameObjectDefinition : *road->getGameObjectDefinitions())
	{
		assert(roadGameObjectDefinition);

		addGameObject(roadGameObjectDefinition->id, roadGameObjectDefinition->wp, roadGameObjectDefinition->offsetX);
	}
}

void ModuleWorld::addAllGameObjects()
{
	assert(road);
	assert(minimap);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleGameObject());

	getGameEngine()->getModuleGameObject()->load();

	gameObjects.reserve(road->getGameObjectsCount() + 1);

	addPlayer();

	addCars();

	// At this point all added game objects are cars (including player)
	// Assign an id to each one

	for(uint i = 0; i < (uint)gameObjects.size(); ++i)
	{
		Car* car = (Car*)gameObjects[i];

		assert(car);

		car->setSpecificId(i);
		minimap->registerCar(car);
	}

	addRoadGameObjects();
}

void ModuleWorld::removeAllGameObjects()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleGameObject());

	player = nullptr;

	gameObjects.clear();

	getGameEngine()->getModuleGameObject()->unload();
}

void ModuleWorld::addCameras()
{
	assert(player);

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
	assert(road);
	assert(camera);
	assert(cameraMirror);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	assert(moduleTexture);

	const RoadBackgroundDefinition* roadBackgroundDefinition = road->getRoadBackgroundDefinition();

	background = new Background(moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), camera->getProjectionY1());
	backgroundMirror = new Background(moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), moduleTexture->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), cameraMirror->getProjectionY1());
}

void ModuleWorld::removeBackgrounds()
{
	if(background)
	{
		delete background; background = nullptr;
	}
	
	if(backgroundMirror)
	{
		delete backgroundMirror; backgroundMirror = nullptr;
	}
}

void ModuleWorld::addRenderingLayers()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	layerRoad = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoad, &viewportRectRoad);
	layerRoadMirror = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoadMirror, &viewportRectRoadMirror);
}

void ModuleWorld::removeRenderingLayers()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->removeLayer(layerRoad);
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoadMirror);

	layerRoad = layerRoadMirror = -1;
}

void ModuleWorld::checkPauseMode()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		paused = !paused;
		pauseCounter = 0.0f;
	}
}

void ModuleWorld::checkGoMenu() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
	{
		assert(getGameEngine()->getModuleSwitch());

		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
	}
}

void ModuleWorld::checkRenderCollisionBoxes() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_F1) == KeyState::DOWN)
		for(GameObject* gameObject : gameObjects)
		{
			assert(gameObject);

			gameObject->setRenderCollisionBox(!gameObject->getRenderCollisionBox());
		}
}

void ModuleWorld::updatePaused(float deltaTimeS)
{
	checkGoMenu();

	updatePauseCounter(deltaTimeS);
}

void ModuleWorld::updateNotPaused(float deltaTimeS)
{
	assert(camera);
	assert(background);
	assert(cameraMirror);
	assert(backgroundMirror);

	updateLightAnimation(deltaTimeS);

	updateLightAnimationFx(deltaTimeS);

	updateLightAnimationMusic(deltaTimeS);

	updateOutRoadFxWaitTime(deltaTimeS);

	if(currentLapTimeNotificate)
		updateCurrentLapTimeNotificationCounter(deltaTimeS);

	for(GameObject* gameObject : gameObjects)
	{
		assert(gameObject);

		gameObject->update(deltaTimeS);
	}

	camera->update(deltaTimeS);
	cameraMirror->update(deltaTimeS);

	background->update(player, road, deltaTimeS);
	backgroundMirror->update(player, road, deltaTimeS);
}

void ModuleWorld::updatePauseCounter(float deltaTimeS)
{
	pauseCounter = mod0L(pauseCounter + deltaTimeS, 1.0f);
}

void ModuleWorld::updateCurrentLapTimeNotificationCounter(float deltaTimeS)
{
	currentLapTimeNotificationCounter += deltaTimeS;
	
	if(currentLapTimeNotificationCounter >= 5.0f)
	{
		currentLapTimeNotificate = false;
		currentLapTimeNotificationCounter = 0.0f;
	}
}

void ModuleWorld::updateLightAnimationFx(float deltaTimeS)
{
	assert(lightAnimation);

	if(!lightSFX0Done && lightAnimation->getTimePercent() >= 0.2f)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleAudio());

		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 0);
		lightSFX0Done = true;
	}

	if(!lightSFX1Done && lightAnimation->getTimePercent() >= 0.6f)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleAudio());

		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 1);
		lightSFX1Done = true;
	}
}

void ModuleWorld::updateLightAnimationMusic(float deltaTimeS)
{
	assert(lightAnimation);

	if(!lightMusicDone && lightAnimation->hasEnded())
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleAudio());

		getGameEngine()->getModuleAudio()->playMusic(audioGroupId, 0, 0.75f);
		lightMusicDone = true;
	}
}

void ModuleWorld::updateLightAnimation(float deltaTimeS) const
{
	assert(lightAnimation);

	if(!lightAnimation->hasEnded())
	{
		lightAnimation->update(deltaTimeS);

		if(lightAnimation->hasEnded())
		{
			for(GameObject* gameObject : gameObjects)
			{
				assert(gameObject);

				if(gameObject->getType() == GameObjectType::CAR || gameObject->getType() == GameObjectType::PLAYER)
					((Car*)gameObject)->setMovementEnabled(true);
			}
		}
	}
}

void ModuleWorld::updateOutRoadFxWaitTime(float deltaTimeS)
{
	if(outRoadFxPlayed && (fxOutRoadCurrentWaitTime += deltaTimeS) >= 0.25f)
	{
		outRoadFxPlayed = false;
		fxOutRoadCurrentWaitTime = 0.0f;
	}
}

void ModuleWorld::render() const
{
	assert(road);
	assert(camera);
	assert(minimap);
	assert(background);
	assert(cameraMirror);
	assert(getGameEngine());
	assert(backgroundMirror);

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	assert(moduleRenderer);

	moduleRenderer->setLayer(layerRoad);
	background->render(!camera->getForward(), moduleRenderer);
	road->render(camera, moduleRenderer);
	minimap->render(moduleRenderer);

	renderUI();

	moduleRenderer->setLayer(layerRoadMirror);
	backgroundMirror->render(!cameraMirror->getForward(), moduleRenderer);
	road->render(cameraMirror, moduleRenderer);
}