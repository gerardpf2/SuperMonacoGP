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

		void playFx(uint idAudioGroup, uint idFx, float volume = 1.0f, int loops = 0) const;

		void playMusic(uint idAudioGroup, uint idMusic, float volume = 1.0f, int loops = -1) const;

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

		/*

		For each audio group
			Vector of musics
			Vector of fxs

		*/

		std::map<uint, std::pair<std::vector<Mix_Music*>*, std::vector<Mix_Chunk*>*>> audioGroups;

		/*

		For each used audio group
			Path to its json file and its corresponding audio group id
			Times loaded counter

		*/

		std::list<std::pair<std::pair<std::string, uint>, uint>> loadedAudioGroups;
};

#endif