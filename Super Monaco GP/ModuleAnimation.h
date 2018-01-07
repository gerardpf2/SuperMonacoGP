#ifndef _MODULE_ANIMATION_
#define _MODULE_ANIMATION_

#include <map>
#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"

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

		virtual void cleanUp() override;

	private:

		bool isAlreadyLoaded(const std::string&, uint& idAnimationGroup) const;

		bool isAlreadyUnloaded(uint idAnimationGroup, std::string& jsonPath) const;

		uint getLoadedCounter(uint idAnimationGroup) const;

		void incrementLoadedCounter(uint idAnimationGroup, int increment);

	private:

		/*

		For each used animation group
			List of the base animations

		*/

		std::map<uint, std::pair<std::vector<std::vector<const Texture*>*>*, std::vector<Animation*>*>> animationGroups;

		/*

		For each used animation group
			List of the base animation containers

		*/

		std::map<uint, std::vector<AnimationContainer*>*> animationContainerGroups;

		/*

		For each used animation group
			Texture group id used by all the animations

		*/

		std::map<uint, uint> animationGroupsTextureGroupId;

		/*

		For each used animation group
			List of the used animations

		*/

		std::map<uint, std::list<Animation*>*> usedAnimations;

		/*

		For each used animation group
			List of the used animation containers

		*/

		std::map<uint, std::list<AnimationContainer*>*> usedAnimationContainers;

		/*

		For each used animation group
			Path to its json file and its corresponding animation group id
			Times loaded counter

		*/

		std::list<std::pair<std::pair<std::string, uint>, uint>> loadedAnimationGroups;
};

#endif