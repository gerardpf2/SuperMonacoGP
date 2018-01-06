#ifndef _MODULE_
#define _MODULE_

class GameEngine;

class Module
{
	public:

		virtual ~Module();

		GameEngine* getGameEngine() const;

		bool getBlocked() const;

		void setBlocked(bool blocked);

		virtual bool setUp();

		virtual bool preUpdate(float deltaTimeS);

		virtual bool update(float deltaTimeS);

		virtual bool postUpdate(float deltaTimeS);

		virtual void cleanUp();

	protected:

		Module(GameEngine* gameEngine);

	private:

		GameEngine* gameEngine = nullptr; // Constructor

		// There is no standard behaviour if blocked
		// Generally it means that the module ignores the update part but it keeps rendering stuff
		// Specific game modules use this feature

		bool blocked = false;
};

#endif