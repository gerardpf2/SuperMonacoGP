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

		// virtual void cleanUp() override;

	private:

		std::map<uint, std::pair<std::vector<std::vector<const Texture*>*>*, std::vector<Animation*>*>> animationGroups;

		std::map<uint, std::vector<AnimationContainer*>*> animationContainerGroups;

		std::map<uint, std::list<Animation*>*> usedAnimations;

		std::map<uint, std::list<AnimationContainer*>*> usedAnimationContainers;

		// std::map<const char*, uint> loadedAnimationGroups;

	/* public:

		ModuleAnimation(GameEngine* gameEngine);

		virtual ~ModuleAnimation();

		Animation* getAnimationCopy(uint animationId);

		AnimationGroup* getAnimationGroupCopy(uint animationGroupId);

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void load(const rapidjson::Document& documentJson);

		void loadAnimationGroups(const rapidjson::Value& animationGroupsJson);

		void loadAnimationGroup(const rapidjson::Value& animationGroupJson);

		void loadAnimations(const rapidjson::Value& animationsJson);

		void loadAnimation(const rapidjson::Value& animationJson);

	private:

		std::vector<Animation*> animations;

		std::list<Animation*> usedAnimations;

		std::vector<AnimationGroup*> animationGroups;

		std::list<AnimationGroup*> usedAnimationGroups; */
};

#endif