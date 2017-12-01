#include "ModuleAnimation.h"

#include "Animation.h"
#include "ModuleJson.h"
#include "GameEngine.h"
#include "ModuleTexture.h"
#include "AnimationContainer.h"

using namespace std;
using namespace rapidjson;

ModuleAnimation::ModuleAnimation(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleAnimation::~ModuleAnimation()
{ }

uint ModuleAnimation::load(const char* jsonPath)
{
	uint tmpAnimationGroupId;
	if(isAlreadyLoaded(jsonPath, tmpAnimationGroupId)) return tmpAnimationGroupId;

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	// id, textureGroupId, animations, animationContainers

	uint animationGroupId = jsonDocument["animationGroupId"].GetUint();
	const char* textureGroupPath = jsonDocument["textureGroupPath"].GetString();
	const Value& animationsJson = jsonDocument["animations"];
	const Value& animationContainersJson = jsonDocument["animationContainers"];

	uint textureGroupId = moduleTexture->load(textureGroupPath);

	usedAnimations[animationGroupId] = new list<Animation*>();

	usedAnimationContainers[animationGroupId] = new list<AnimationContainer*>();

	vector<vector<const Texture*>*>* animationGroupsTextures = new vector<vector<const Texture*>*>();
	animationGroupsTextures->reserve(animationsJson.Size());

	vector<Animation*>* animations = new vector<Animation*>();
	animations->reserve(animationsJson.Size());

	for(SizeType i = 0; i < animationsJson.Size(); ++i)
	{
		// animationId, loop, endTime, textureIds

		uint animationId = i;
		bool loop = animationsJson[i]["loop"].GetBool();
		float endTime = animationsJson[i]["endTime"].GetFloat();
		const Value& textureIdsJson = animationsJson[i]["textureIds"];

		vector<const Texture*>* animationTextures = new vector<const Texture*>();
		animationTextures->reserve(textureIdsJson.Size());

		for(SizeType i = 0; i < textureIdsJson.Size(); ++i)
		{
			// textureId

			uint textureId = textureIdsJson[i].GetUint();

			animationTextures->push_back(moduleTexture->get(textureGroupId, textureId));
		}

		animationGroupsTextures->push_back(animationTextures);

		Animation* animation = new Animation(animationId, endTime, loop, animationTextures);

		animations->push_back(animation);
	}

	animationGroups[animationGroupId] = pair<vector<vector<const Texture*>*>*, vector<Animation*>*>(animationGroupsTextures, animations);

	// AnimationContainers

	vector<AnimationContainer*>* animationContainerGroupsAnimationIds = new vector<AnimationContainer*>();
	animationContainerGroupsAnimationIds->reserve(animationsJson.Size());

	for(SizeType i = 0; i < animationContainersJson.Size(); ++i)
	{
		// animationContainerId, currentId, animationIds

		uint animationContainerId = i;
		const Value& animationIdsJson = animationContainersJson[i]["animationIds"];

		vector<uint>* animationIds = new vector<uint>();
		animationIds->reserve(animationIdsJson.Size());

		for(SizeType i = 0; i < animationIdsJson.Size(); ++i)
		{
			// animationId

			uint animationId = animationIdsJson[i].GetUint();

			animationIds->push_back(animationId);
		}

		map<uint, Animation*>* animations = new map<uint, Animation*>();

		for(uint i = 0; i < animationIds->size(); ++i)
			(*animations)[(*animationIds)[i]] = getAnimation(animationGroupId, (*animationIds)[i]);

		AnimationContainer* animationContainer = new AnimationContainer(animationContainerId, animationGroupId, (*animationIds)[0], animations);

		animationIds->clear();

		delete animationIds; animationIds = nullptr;

		animationContainerGroupsAnimationIds->push_back(animationContainer);
	}

	animationContainerGroups[animationGroupId] = animationContainerGroupsAnimationIds;

	animationGroupsTextureGroupId[animationGroupId] = textureGroupId;

	loadedAnimationGroups.push_back(pair<const char*, uint>(jsonPath, animationGroupId));

	return animationGroupId;
}

void ModuleAnimation::unload(uint idAnimationGroup)
{
	const char* tmpJsonPath;
	if(isAlreadyUnloaded(idAnimationGroup, tmpJsonPath)) return;

	pair<vector<vector<const Texture*>*>*, vector<Animation*>*>& animationGroup = animationGroups[idAnimationGroup];

	vector<vector<const Texture*>*>*& animationGroupsTextures = animationGroup.first;

	vector<Animation*>*& animations = animationGroup.second;

	for(int i = (int)animationGroupsTextures->size() - 1; i >= 0; --i)
	{
		(*animationGroupsTextures)[i]->clear();
		
		delete (*animationGroupsTextures)[i];
		(*animationGroupsTextures)[i] = nullptr;
	}

	animationGroupsTextures->clear();
	
	delete animationGroupsTextures;
	animationGroupsTextures = nullptr;

	for(int i = (int)animations->size() - 1; i >= 0; --i)
	{
		(*animations)[i]->cleanUp();

		delete (*animations)[i];
		(*animations)[i] = nullptr;
	}

	animations->clear();
	
	delete animations;
	animations = nullptr;

	// Unload animationContainers

	for(map<uint, vector<AnimationContainer*>*>::reverse_iterator it = animationContainerGroups.rbegin(); it != animationContainerGroups.rend(); ++it)
	{
		for(int i = (int)it->second->size() - 1; i >= 0; --i)
		{
			(*(it->second))[i]->cleanUp();

			delete (*(it->second))[i];
			(*(it->second))[i] = nullptr;
		}

		it->second->clear();

		delete it->second; it->second = nullptr;
	}

	animationContainerGroups.clear();

	animationGroups.erase(idAnimationGroup);

	animationContainerGroups.erase(idAnimationGroup);

	list<Animation*>*& usedAnimationsOfGroup = usedAnimations.at(idAnimationGroup);

	for(list<Animation*>::reverse_iterator it = usedAnimationsOfGroup->rbegin(); it != usedAnimationsOfGroup->rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimationsOfGroup->clear();

	delete usedAnimationsOfGroup; usedAnimationsOfGroup = nullptr;

	usedAnimations.erase(idAnimationGroup);

	list<AnimationContainer*>*& usedAnimationContainersOfGroup = usedAnimationContainers.at(idAnimationGroup);

	for(list<AnimationContainer*>::reverse_iterator it = usedAnimationContainersOfGroup->rbegin(); it != usedAnimationContainersOfGroup->rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimationContainersOfGroup->clear();

	delete usedAnimationContainersOfGroup; usedAnimationContainersOfGroup = nullptr;

	usedAnimationContainers.erase(idAnimationGroup);

	getGameEngine()->getModuleTexture()->unload(animationGroupsTextureGroupId[idAnimationGroup]);

	animationGroupsTextureGroupId.erase(idAnimationGroup);

	loadedAnimationGroups.remove(pair<const char*, uint>(tmpJsonPath, idAnimationGroup));
}

Animation* ModuleAnimation::getAnimation(uint idAnimationGroup, uint idAnimation) const
{
	Animation* animation = new Animation(*(*animationGroups.at(idAnimationGroup).second)[idAnimation]);

	usedAnimations.at(idAnimationGroup)->push_back(animation);

	return animation;
}

AnimationContainer* ModuleAnimation::getAnimationContainer(uint idAnimationGroup, uint idAnimationContainer) const
{
	const AnimationContainer* baseAnimationContainer = (*animationContainerGroups.at(idAnimationGroup))[idAnimationContainer];

	const map<uint, Animation*>* baseAnimations = baseAnimationContainer->getAnimations();

	map<uint, Animation*>* animations = new map<uint, Animation*>();

	for(map<uint, Animation*>::const_iterator it = baseAnimations->begin(); it != baseAnimations->end(); ++it)
		(*animations)[it->first] = getAnimation(idAnimationGroup, it->second->getId());

	AnimationContainer* animationContainer = new AnimationContainer(baseAnimationContainer->getId(), baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getCurrentAnimationId(), animations);

	usedAnimationContainers.at(idAnimationGroup)->push_back(animationContainer);

	return animationContainer;
}

bool ModuleAnimation::isAlreadyLoaded(const char* jsonPath, uint& idAnimationGroup) const
{
	for(list<pair<const char*, uint>>::const_iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(strcmp(it->first, jsonPath) == 0)
		{
			idAnimationGroup = it->second;

			return true;
		}

	return false;
}

bool ModuleAnimation::isAlreadyUnloaded(uint idAnimationGroup, const char*& jsonPath) const
{
	for(list<pair<const char*, uint>>::const_iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(it->second == idAnimationGroup)
		{
			jsonPath = it->first;

			return false;
		}

	return true;
}