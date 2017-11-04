#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include "Module.h"

class Road;
class Camera;

class ModuleWorld : public Module
{
	public:

		ModuleWorld(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleWorld();

		virtual bool setUp() override;

		virtual bool preUpdate() override;

		virtual bool update(float deltaTimeS) override;

		virtual bool postUpdate() override;

		virtual void cleanUp() override;

	private:

		Road* road = nullptr;

		Camera* camera = nullptr;
};

#endif