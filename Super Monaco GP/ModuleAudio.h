#ifndef _MODULE_AUDIO_
#define _MODULE_AUDIO_

#include <map>
#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"

struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
	public:

		ModuleAudio(GameEngine* gameEngine);

		virtual ~ModuleAudio();

		uint load(const char* jsonPath);

		void unload(uint idAudioGroup);

		void playFx(uint idAudioGroup, uint idFx, int loops = 0) const;

		void playMusic(uint idAudioGroup, uint idMusic, int loops = -1) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		Mix_Music* loadMusic(const std::string& path);

		void unloadMusic(Mix_Music*& music);

		Mix_Chunk* loadFx(const std::string& path);

		void unloadFx(Mix_Chunk*& fx);

		bool isAlreadyLoaded(const std::string& jsonPath, uint& idAudioGroup) const;

		bool isAlreadyUnloaded(uint idAudioGroup, std::string& jsonPath) const;

		uint getLoadedCounter(uint idAudioGroup) const;

		void incrementLoadedCounter(uint idAudioGroup, int increment);

	private:

		std::map<uint, std::pair<std::vector<Mix_Music*>*, std::vector<Mix_Chunk*>*>> audioGroups;

		std::list<std::pair<std::pair<std::string, uint>, uint>> loadedAudioGroups;
};

#endif