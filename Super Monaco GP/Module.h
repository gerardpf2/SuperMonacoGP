#ifndef _MODULE_
#define _MODULE_

class GameEngine;

class Module
{
	public:

		virtual ~Module();

		GameEngine* getGameEngine() const;

		virtual bool setUp();

		virtual bool preUpdate(float deltaTimeS);

		virtual bool update(float deltaTimeS);

		virtual bool postUpdate(float deltaTimeS);

		virtual void cleanUp();

	protected:

		Module(GameEngine* gameEngine);

	private:

		GameEngine* gameEngine = nullptr;
};

#endif