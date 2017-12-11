#ifndef _MODULE_ANIMATION_
#define _MODULE_ANIMATION_

#include <map>
#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"

struct Texture;

class Animation;
class AnimationContainer;

class ModuleAnimation : public Module
{
	public:

		ModuleAnimation(GameEngine* gameEngine);

		virtual ~ModuleAnimation();

		uint load(const char* jsonPath);

		void unload(uint idAnimationGroup);

		Animation* getAnimation(uint idAnimationGroup, uint idAnimation) const;

		AnimationContainer* getAnimationContainer(uint idAnimationGroup, uint idAnimationContainer) const;

	private:

		bool isAlreadyLoaded(const std::string&, uint& idAnimationGroup) const;

		bool isAlreadyUnloaded(uint idAnimationGroup, std::string& jsonPath) const;

		uint getLoadedCounter(uint idAnimationGroup) const;

		void incrementLoadedCounter(uint idAnimationGroup, int increment);

	private:

		std::map<uint, std::pair<std::vector<std::vector<const Texture*>*>*, std::vector<Animation*>*>> animationGroups;

		std::map<uint, std::vector<AnimationContainer*>*> animationContainerGroups;

		std::map<uint, uint> animationGroupsTextureGroupId;

		std::map<uint, std::list<Animation*>*> usedAnimations;

		std::map<uint, std::list<AnimationContainer*>*> usedAnimationContainers;

		std::list<std::pair<std::pair<std::string, uint>, uint>> loadedAnimationGroups;
};

#endif