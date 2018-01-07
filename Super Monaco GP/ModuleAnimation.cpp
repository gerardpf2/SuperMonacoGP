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
	assert(jsonPath);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleJson());

	uint tmpAnimationGroupId;
	if(isAlreadyLoaded(jsonPath, tmpAnimationGroupId))
	{
		incrementLoadedCounter(tmpAnimationGroupId, 1);
		return tmpAnimationGroupId;
	}

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	assert(moduleTexture);

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

	loadedAnimationGroups.push_back(pair<pair<string, uint>, uint>(pair<string, uint>(jsonPath, animationGroupId), 1));

	return animationGroupId;
}

void ModuleAnimation::unload(uint idAnimationGroup)
{
	string tmpJsonPath;
	if(isAlreadyUnloaded(idAnimationGroup, tmpJsonPath)) return;

	incrementLoadedCounter(idAnimationGroup, -1);
	if(getLoadedCounter(idAnimationGroup) > 0) return;

	map<uint, pair<vector<vector<const Texture*>*>*, vector<Animation*>*>>::iterator it0 = animationGroups.find(idAnimationGroup);

	assert(it0 != animationGroups.end());

	pair<vector<vector<const Texture*>*>*, vector<Animation*>*>& animationGroup = it0->second;

	vector<vector<const Texture*>*>*& animationGroupsTextures = animationGroup.first;

	vector<Animation*>*& animations = animationGroup.second;

	assert(animationGroupsTextures);
	assert(animations);

	// Unload animation group textures

	for(int i = (int)animationGroupsTextures->size() - 1; i >= 0; --i)
	{
		assert(animationGroupsTextures->at(i));

		animationGroupsTextures->at(i)->clear();

		delete animationGroupsTextures->at(i); animationGroupsTextures->at(i) = nullptr;
	}

	animationGroupsTextures->clear();
	
	delete animationGroupsTextures; animationGroupsTextures = nullptr;

	// Unload animation group animations

	for(int i = (int)animations->size() - 1; i >= 0; --i)
	{
		assert(animations->at(i));

		animations->at(i)->cleanUp();

		delete animations->at(i); animations->at(i) = nullptr;
	}

	animations->clear();
	
	delete animations; animations = nullptr;

	animationGroups.erase(idAnimationGroup);

	// Unload animation containers

	map<uint, vector<AnimationContainer*>*>::iterator it1 = animationContainerGroups.find(idAnimationGroup);

	assert(it1 != animationContainerGroups.end());

	vector<AnimationContainer*>*& animationContainerGroup = it1->second;

	assert(animationContainerGroup);

	for(int i = (int)animationContainerGroup->size() - 1; i >= 0; --i)
	{
		assert(animationContainerGroup->at(i));

		animationContainerGroup->at(i)->cleanUp();

		delete animationContainerGroup->at(i); animationContainerGroup->at(i) = nullptr;
	}

	animationContainerGroup->clear();

	delete animationContainerGroup; animationContainerGroup = nullptr;

	animationContainerGroups.erase(idAnimationGroup);

	// Unload used animations

	map<uint, list<Animation*>*>::iterator it2 = usedAnimations.find(idAnimationGroup);

	assert(it2 != usedAnimations.end());

	list<Animation*>*& usedAnimationsOfGroup = it2->second;

	assert(usedAnimationsOfGroup);

	for(list<Animation*>::reverse_iterator it = usedAnimationsOfGroup->rbegin(); it != usedAnimationsOfGroup->rend(); ++it)
	{
		assert(*it);

		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimationsOfGroup->clear();

	delete usedAnimationsOfGroup; usedAnimationsOfGroup = nullptr;

	usedAnimations.erase(idAnimationGroup);

	// Unload used animation containers

	map<uint, list<AnimationContainer*>*>::iterator it3 = usedAnimationContainers.find(idAnimationGroup);

	assert(it3 != usedAnimationContainers.end());

	list<AnimationContainer*>*& usedAnimationContainersOfGroup = it3->second;

	assert(usedAnimationContainersOfGroup);

	for(list<AnimationContainer*>::reverse_iterator it = usedAnimationContainersOfGroup->rbegin(); it != usedAnimationContainersOfGroup->rend(); ++it)
	{
		assert(*it);

		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimationContainersOfGroup->clear();

	delete usedAnimationContainersOfGroup; usedAnimationContainersOfGroup = nullptr;

	usedAnimationContainers.erase(idAnimationGroup);

	// Unload associated texture group

	map<uint, uint>::iterator it4 = animationGroupsTextureGroupId.find(idAnimationGroup);

	assert(getGameEngine());
	assert(getGameEngine()->getModuleTexture());
	assert(it4 != animationGroupsTextureGroupId.end());

	getGameEngine()->getModuleTexture()->unload(it4->second);

	animationGroupsTextureGroupId.erase(idAnimationGroup);

	loadedAnimationGroups.remove(pair<pair<string, uint>, uint>(pair<string, uint>(tmpJsonPath, idAnimationGroup), 0));
}

Animation* ModuleAnimation::getAnimation(uint idAnimationGroup, uint idAnimation) const
{
	map<uint, pair<vector<vector<const Texture*>*>*, vector<Animation*>*>>::const_iterator it0 = animationGroups.find(idAnimationGroup);

	assert(it0 != animationGroups.end());
	assert(it0->second.second);
	assert((uint)it0->second.second->size() > idAnimation);

	Animation* animation = new Animation(*it0->second.second->at(idAnimation));

	map<uint, list<Animation*>*>::const_iterator it1 = usedAnimations.find(idAnimationGroup);

	assert(it1 != usedAnimations.end());
	assert(it1->second);

	it1->second->push_back(animation);

	return animation;
}

AnimationContainer* ModuleAnimation::getAnimationContainer(uint idAnimationGroup, uint idAnimationContainer) const
{
	map<uint, vector<AnimationContainer*>*>::const_iterator it0 = animationContainerGroups.find(idAnimationGroup);

	assert(it0 != animationContainerGroups.end());
	assert(it0->second);
	assert((uint)it0->second->size() > idAnimationContainer);

	const AnimationContainer* baseAnimationContainer = it0->second->at(idAnimationContainer);

	assert(baseAnimationContainer);

	const map<uint, Animation*>* baseAnimations = baseAnimationContainer->getAnimations();

	assert(baseAnimations);

	map<uint, Animation*>* animations = new map<uint, Animation*>();

	for(map<uint, Animation*>::const_iterator it = baseAnimations->begin(); it != baseAnimations->end(); ++it)
	{
		assert(it->second);

		(*animations)[it->first] = getAnimation(idAnimationGroup, it->second->getId());
	}

	AnimationContainer* animationContainer = new AnimationContainer(baseAnimationContainer->getId(), baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getCurrentAnimationId(), animations);

	map<uint, list<AnimationContainer*>*>::const_iterator it1 = usedAnimationContainers.find(idAnimationGroup);

	assert(it1 != usedAnimationContainers.end());
	assert(it1->second);

	it1->second->push_back(animationContainer);

	return animationContainer;
}

void ModuleAnimation::cleanUp()
{
	animationGroups.clear();

	animationContainerGroups.clear();

	animationGroupsTextureGroupId.clear();

	usedAnimations.clear();

	usedAnimationContainers.clear();

	loadedAnimationGroups.clear();
}

bool ModuleAnimation::isAlreadyLoaded(const string& jsonPath, uint& idAnimationGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(it->first.first == jsonPath)
		{
			idAnimationGroup = it->first.second;

			return true;
		}

	return false;
}

bool ModuleAnimation::isAlreadyUnloaded(uint idAnimationGroup, string& jsonPath) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(it->first.second == idAnimationGroup)
		{
			jsonPath = it->first.first;

			return false;
		}

	return true;
}

uint ModuleAnimation::getLoadedCounter(uint idAnimationGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(it->first.second == idAnimationGroup) return it->second;

	return 0;
}

void ModuleAnimation::incrementLoadedCounter(uint idAnimationGroup, int increment)
{
	for(list<pair<pair<string, uint>, uint>>::iterator it = loadedAnimationGroups.begin(); it != loadedAnimationGroups.end(); ++it)
		if(it->first.second == idAnimationGroup)
		{
			it->second += increment;

			break;
		}
}