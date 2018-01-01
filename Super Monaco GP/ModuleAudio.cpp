#include "ModuleAudio.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include "ModuleJson.h"
#include "GameEngine.h"

using namespace std;
using namespace rapidjson;

ModuleAudio::ModuleAudio(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleAudio::~ModuleAudio()
{ }

uint ModuleAudio::load(const char* jsonPath)
{
	uint tmpAudioGroupId;
	if(isAlreadyLoaded(jsonPath, tmpAudioGroupId))
	{
		incrementLoadedCounter(tmpAudioGroupId, 1);
		return tmpAudioGroupId;
	}

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	// id, musics, fxs

	uint audioGroupId = jsonDocument["audioGroupId"].GetUint();
	const Value& musicsJson = jsonDocument["musics"];
	const Value& fxsJson = jsonDocument["fxs"];

	vector<Mix_Music*>* musics = new vector<Mix_Music*>();
	musics->reserve(musicsJson.Size());

	for(SizeType i = 0; i < musicsJson.Size(); ++i)
	{
		// path

		const char* path = musicsJson[i].GetString();

		musics->push_back(loadMusic(path));
	}

	vector<Mix_Chunk*>* fxs = new vector<Mix_Chunk*>();
	fxs->reserve(fxsJson.Size());

	for(SizeType i = 0; i < fxsJson.Size(); ++i)
	{
		// path

		const char* path = fxsJson[i].GetString();

		fxs->push_back(loadFx(path));
	}

	audioGroups[audioGroupId] = pair<vector<Mix_Music*>*, vector<Mix_Chunk*>*>(musics, fxs);

	loadedAudioGroups.push_back(pair<pair<string, uint>, uint>(pair<string, uint>(jsonPath, audioGroupId), 1));

	return audioGroupId;
}

void ModuleAudio::unload(uint idAudioGroup)
{
	string tmpJsonPath;
	if(isAlreadyUnloaded(idAudioGroup, tmpJsonPath)) return;

	incrementLoadedCounter(idAudioGroup, -1);
	if(getLoadedCounter(idAudioGroup) > 0) return;

	pair<vector<Mix_Music*>*, vector<Mix_Chunk*>*>& audioGroup = audioGroups[idAudioGroup];

	for(Mix_Music* music : *audioGroup.first) unloadMusic(music);
	for(Mix_Chunk* fx : *audioGroup.second) unloadFx(fx);

	audioGroup.first->clear();
	delete audioGroup.first; audioGroup.first = nullptr;

	audioGroup.second->clear();
	delete audioGroup.second; audioGroup.second = nullptr;

	audioGroups.erase(idAudioGroup);

	loadedAudioGroups.remove(pair<pair<string, uint>, uint>(pair<string, uint>(tmpJsonPath, idAudioGroup), 0));
}

void ModuleAudio::playFx(uint idAudioGroup, uint idFx, int loops) const
{
	/* uint left = 20;
	uint distance = 100;

	Mix_SetPanning(MIX_CHANNEL_POST, left, 254 - left);
	Mix_SetDistance(MIX_CHANNEL_POST, distance); */

	Mix_PlayChannel(-1, audioGroups.at(idAudioGroup).second->at(idFx), loops);
}

void ModuleAudio::playMusic(uint idAudioGroup, uint idMusic, int loops) const
{
	Mix_PlayMusic(audioGroups.at(idAudioGroup).first->at(idMusic), loops);
}

bool ModuleAudio::setUp()
{
	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		return false;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		return false;
	}

	return true;
}

void ModuleAudio::cleanUp()
{
	audioGroups.clear();

	loadedAudioGroups.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

Mix_Music* ModuleAudio::loadMusic(const std::string& path)
{
	return Mix_LoadMUS(path.c_str());
}

void ModuleAudio::unloadMusic(Mix_Music*& music)
{
	if(music)
	{
		Mix_FreeMusic(music);
		music = nullptr;
	}
}

Mix_Chunk* ModuleAudio::loadFx(const std::string& path)
{
	return Mix_LoadWAV(path.c_str());
}

void ModuleAudio::unloadFx(Mix_Chunk*& fx)
{
	if(fx)
	{
		Mix_FreeChunk(fx);
		fx = nullptr;
	}
}

bool ModuleAudio::isAlreadyLoaded(const std::string& jsonPath, uint& idAudioGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAudioGroups.begin(); it != loadedAudioGroups.end(); ++it)
		if(it->first.first == jsonPath)
		{
			idAudioGroup = it->first.second;

			return true;
		}

	return false;
}

bool ModuleAudio::isAlreadyUnloaded(uint idAudioGroup, std::string& jsonPath) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAudioGroups.begin(); it != loadedAudioGroups.end(); ++it)
		if(it->first.second == idAudioGroup)
		{
			jsonPath = it->first.first;

			return false;
		}

	return true;
}

uint ModuleAudio::getLoadedCounter(uint idAudioGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAudioGroups.begin(); it != loadedAudioGroups.end(); ++it)
		if(it->first.second == idAudioGroup) return it->second;

	return 0;
}

void ModuleAudio::incrementLoadedCounter(uint idAudioGroup, int increment)
{
	for(list<pair<pair<string, uint>, uint>>::iterator it = loadedAudioGroups.begin(); it != loadedAudioGroups.end(); ++it)
		if(it->first.second == idAudioGroup)
		{
			it->second += increment;

			break;
		}
}