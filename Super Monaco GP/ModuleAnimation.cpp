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
	// Check if already loaded

	// map<const char*, uint>::iterator it = loadedAnimationGroups.find(jsonPath);
	// if(it != loadedAnimationGroups.end()) return it->second;

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	// id, textureGroupId, animations, animationContainers

	uint animationGroupId = jsonDocument["animationGroupId"].GetUint();
	const char* textureGroupPath = jsonDocument["textureGroupPath"].GetString();
	const Value& animationsJson = jsonDocument["animations"];
	const Value& animationContainersJson = jsonDocument["animationContainers"];

	unload(animationGroupId);

	uint textureGroupId = moduleTexture->load(textureGroupPath);

	usedAnimations[animationGroupId] = new list<Animation*>();

	usedAnimationContainers[animationGroupId] = new list<AnimationContainer*>();

	vector<vector<const Texture*>*>* animationGroupsTextures = new vector<vector<const Texture*>*>(); // animationsJson.Size(), nullptr);
	animationGroupsTextures->reserve(animationsJson.Size());

	vector<Animation*>* animations = new vector<Animation*>(); // animationsJson.Size(), nullptr);
	animations->reserve(animationsJson.Size());

	for(SizeType i = 0; i < animationsJson.Size(); ++i)
	{
		// animationId, loop, endTime, textureIds

		uint animationId = i; // animationsJson[i]["animationId"].GetUint();
		bool loop = animationsJson[i]["loop"].GetBool();
		// bool inverse = animationsJson[i]["inverse"].GetBool();
		float endTime = animationsJson[i]["endTime"].GetFloat();
		const Value& textureIdsJson = animationsJson[i]["textureIds"];

		vector<const Texture*>* animationTextures = new vector<const Texture*>(); // textureIdsJson.Size(), nullptr);
		animationTextures->reserve(textureIdsJson.Size());

		for(SizeType i = 0; i < textureIdsJson.Size(); ++i)
		{
			// textureId

			uint textureId = textureIdsJson[i].GetUint();

			// (*animationTextures)[i] = moduleTexture->get(textureGroupId, textureId);
			animationTextures->push_back(moduleTexture->get(textureGroupId, textureId));
		}

		// (*animationGroupsTextures)[animationId] = animationTextures;
		animationGroupsTextures->push_back(animationTextures);

		Animation* animation = new Animation(animationId, animationTextures, endTime, loop /* , inverse */);

		// (*animations)[animationId] = animation;
		animations->push_back(animation);
	}

	animationGroups[animationGroupId] = pair<vector<vector<const Texture*>*>*, vector<Animation*>*>(animationGroupsTextures, animations);

	// AnimationContainers

	vector<AnimationContainer*>* animationContainerGroupsAnimationIds = new vector<AnimationContainer*>(); // animationsJson.Size(), nullptr);
	animationContainerGroupsAnimationIds->reserve(animationsJson.Size());

	for(SizeType i = 0; i < animationContainersJson.Size(); ++i)
	{
		// animationContainerId, currentId, animationIds

		uint animationContainerId = i; // animationContainersJson[i]["animationContainerId"].GetUint();
		// uint currentId = animationContainersJson[i]["currentId"].GetUint();
		const Value& animationIdsJson = animationContainersJson[i]["animationIds"];

		vector<uint>* animationIds = new vector<uint>(); // animationIdsJson.Size());
		animationIds->reserve(animationIdsJson.Size());

		for(SizeType i = 0; i < animationIdsJson.Size(); ++i)
		{
			// animationId

			uint animationId = animationIdsJson[i].GetUint();

			// (*animationIds)[i] = animationId;
			animationIds->push_back(animationId);
		}

		map<uint, Animation*>* animations = new map<uint, Animation*>();

		for(uint i = 0; i < animationIds->size(); ++i)
			(*animations)[(*animationIds)[i]] = getAnimation(animationGroupId, (*animationIds)[i]);

		AnimationContainer* animationContainer = new AnimationContainer(animationContainerId, (*animationIds)[0], animations);

		animationIds->clear();

		delete animationIds; animationIds = nullptr;

		// (*animationContainerGroupsAnimationIds)[animationContainerId] = animationContainer;
		animationContainerGroupsAnimationIds->push_back(animationContainer);
	}

	animationContainerGroups[animationGroupId] = animationContainerGroupsAnimationIds;

	// loadedAnimationGroups[jsonPath] = textureGroupId;

	return animationGroupId;
}

void ModuleAnimation::unload(uint idAnimationGroup)
{
	// Check if already unloaded

	if(animationGroups.find(idAnimationGroup) == animationGroups.end()) return;

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

	// loadedAnimationGroups.clear();

	getGameEngine()->getModuleTexture()->unload(idAnimationGroup);
}

Animation* ModuleAnimation::getAnimation(uint idAnimationGroup, uint idAnimation) const
{
	Animation* animation = new Animation(*(*animationGroups.at(idAnimationGroup).second)[idAnimation]);

	// usedAnimations.push_back(animation);
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

	AnimationContainer* animationContainer = new AnimationContainer(baseAnimationContainer->getId(), baseAnimationContainer->getCurrentAnimationId(), animations);

	// usedAnimationContainers.push_back(animationContainer);
	usedAnimationContainers.at(idAnimationGroup)->push_back(animationContainer);

	return animationContainer;
}

/* void ModuleAnimation::cleanUp()
{
	for(list<Animation*>::reverse_iterator it = usedAnimations.rbegin(); it != usedAnimations.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimations.clear();

	for(list<AnimationContainer*>::reverse_iterator it = usedAnimationContainers.rbegin(); it != usedAnimationContainers.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedAnimationContainers.clear();
} */

/* #include "Animation.h"
#include "GameEngine.h"
#include "ModuleJson.h"
#include "AnimationGroup.h"

using namespace std;
using namespace rapidjson;

ModuleAnimation::ModuleAnimation(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleAnimation::~ModuleAnimation()
{ }

Animation* ModuleAnimation::getAnimationCopy(uint animationId)
{
	Animation* animationCopy = new Animation(*animations[animationId]);
	usedAnimations.push_back(animationCopy);

	return animationCopy;
}

AnimationGroup* ModuleAnimation::getAnimationGroupCopy(uint animationGroupId)
{
	AnimationGroup* animationGroupCopy = new AnimationGroup(*animationGroups[animationGroupId], this);
	usedAnimationGroups.push_back(animationGroupCopy);

	return animationGroupCopy;
}

bool ModuleAnimation::setUp()
{
	Document documentJson;
	getGameEngine()->getModuleJson()->read("Resources/Configuration/Animations.json", documentJson);

	load(documentJson);

	return true;
}

void ModuleAnimation::cleanUp()
{
	for(int i = (int)animations.size() - 1; i >= 0; --i)
	{
		animations[i]->cleanUp(true);

		delete animations[i];
		animations[i] = nullptr;
	}

	animations.clear();

	for(list<Animation*>::reverse_iterator it = usedAnimations.rbegin(); it != usedAnimations.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it;
		*it = nullptr;
	}

	usedAnimations.clear();

	for(int i = (int)animationGroups.size() - 1; i >= 0; --i)
	{
		animationGroups[i]->cleanUp();

		delete animationGroups[i];
		animationGroups[i] = nullptr;
	}

	animationGroups.clear();

	for(list<AnimationGroup*>::reverse_iterator it = usedAnimationGroups.rbegin(); it != usedAnimationGroups.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it;
		*it = nullptr;
	}

	usedAnimationGroups.clear();
}

void ModuleAnimation::load(const Document& documentJson)
{
	// Animations must be loaded first

	loadAnimations(documentJson["animations"]);
	loadAnimationGroups(documentJson["animationGroups"]);
}

void ModuleAnimation::loadAnimationGroups(const Value& animationGroupsJson)
{
	animationGroups.resize(animationGroupsJson.Size(), nullptr);

	for(SizeType i = 0; i < animationGroupsJson.Size(); ++i)
		loadAnimationGroup(animationGroupsJson[i]);
}

void ModuleAnimation::loadAnimationGroup(const Value& animationGroupJson)
{
	AnimationGroup* animationGroup = new AnimationGroup(animationGroupJson, this);
	animationGroups[animationGroup->getId()] = animationGroup;
}

void ModuleAnimation::loadAnimations(const Value& animationsJson)
{
	animations.resize(animationsJson.Size(), nullptr);

	for(SizeType i = 0; i < animationsJson.Size(); ++i)
		loadAnimation(animationsJson[i]);
}

void ModuleAnimation::loadAnimation(const Value& animationJson)
{
	Animation* animation = new Animation(animationJson, getGameEngine()->getModuleTexture());
	animations[animation->getId()] = animation;
} */