#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include <vector>
#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>
#include "ModuleWorldUI.h"

class Car;
class Road;
class Camera;
class Player;
class Minimap;
class Animation;
class Background;
class GameObject;

class ModuleWorld : public Module
{
	public:

		virtual ~ModuleWorld();

		const Road* getRoad() const;

		const Player* getPlayer() const;

		const Camera* getCamera() const;

		const Camera* getCameraMirror() const;

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

		virtual void registerLapTime(const Car* car);

		void playFxOutRoad();

		void playFxCollision() const;

	protected:

		ModuleWorld(GameEngine* gameEngine);

		virtual void addPlayer();

		virtual void addCars();

		virtual void renderUI() const;

		GameObject* addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad = 0.0f);

	private:

		void addRoad();

		void removeRoad();

		void addMinimap();

		void removeMinimap();

		void addRoadGameObjects();

		void addAllGameObjects();

		void removeAllGameObjects();

		void addCameras();

		void removeCameras();

		void addBackgrounds();

		void removeBackgrounds();

		void addRenderingLayers();

		void removeRenderingLayers();

		void checkPauseMode();

		void checkGoMenu() const;

		void checkRenderCollisionBoxes() const;

		void updatePaused(float deltaTimeS);

		void updateNotPaused(float deltaTimeS);

		void updatePauseCounter(float deltaTimeS);

		void updateCurrentLapTimeNotificationCounter(float deltaTimeS);

		void updateLightAnimationFx(float deltaTimeS);

		void updateLightAnimationMusic(float deltaTimeS);

		void updateLightAnimation(float deltaTimeS) const;

		void updateOutRoadFxWaitTime(float deltaTimeS);

		// void updateCollisionFxWaitTime(float deltaTimeS);

		void render() const;

	protected:

		bool paused = false;

		Player* player = nullptr;

		bool renderCurrentLapTime = true;

	private:

		Road* road = nullptr;

		Camera* camera = nullptr;

		Camera* cameraMirror = nullptr;

		std::vector<GameObject*> gameObjects;

		Minimap* minimap = nullptr;

		Background* background = nullptr;

		Background* backgroundMirror = nullptr;

		float pauseCounter = 0.0;

		const Texture* kmhT = nullptr;

		uint audioGroupId = -1;

		bool lightSFX0Done = false;

		bool lightSFX1Done = false;

		bool lightMusicDone = false;

		bool outRoadFxPlayed = false;

		// bool collisionFxPlayed = false;

		float fxOutRoadCurrentWaitTime = 0.0f;

		// float fxCollisionCurrentWaitTime = 0.0f;

		// float currentLapTime = 0.0f;

		// Rendering layers and UI

		uint layerRoad, layerRoadMirror;

		SDL_Rect textureRectRoad, viewportRectRoad;

		SDL_Rect textureRectRoadMirror, viewportRectRoadMirror;

		SDL_Rect kmhRect;

		WindowPosition pausePosition;

		WindowPosition goMenuPosition;

		WindowPosition bestLapPosition;

		WindowPosition bestLapValuePosition;

		WindowPosition currentLapTimePosition;

		bool currentLapTimeNotificate = false;

		float currentLapTimeNotificationCounter = 0.0;

		WindowPosition currentLapTimeNotificationPosition;

		WindowPosition currentLapTimeNotificationValuePosition;

		WindowPosition currentVelocityPosition;

		WindowTrapezoid mirrorBorderTrapezoidLeft, mirrorBorderTrapezoidBottom, mirrorBorderTrapezoidRight, mirrorBorderTrapezoidTop;

		SDL_Rect lightRect;

		uint lightIdAnimationGroup;

		Animation* lightAnimation = nullptr;
};

#endif