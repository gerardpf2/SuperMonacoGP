#ifndef _MODULE_
#define _MODULE_

class GameEngine;

class Module
{
	public:

		virtual ~Module();

		virtual bool setUp();

		virtual bool preUpdate(float deltaTimeS);

		virtual bool update(float deltaTimeS);

		virtual bool postUpdate(float deltaTimeS);

		virtual void cleanUp();

		GameEngine* getGameEngine() const;

		bool getActive() const;

		void setActive(bool active);

	protected:

		Module(GameEngine* gameEngine, bool active);

	private:

		GameEngine* gameEngine = nullptr;

		bool active;
};

#endif