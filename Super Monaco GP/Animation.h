#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include "Types.h"
#include "rapidjson/document.h"

class ModuleTexture;
enum class AnimationType;

class Animation
{
	public:

		Animation(const Animation& animation);
		Animation(const rapidjson::Value& value, const ModuleTexture* moduleTexture);

		virtual ~Animation();

		AnimationType getType() const;

		const std::vector<const Texture*>* getTextures() const;

		float getEndTime() const;

		bool getLoop() const;

		bool hasEnded() const;

		const Texture* getCurrentFrame() const;

		float getTimeMultiplier() const;

		void setTimeMultiplier(float timeMultiplier);

		void reset();

		void update(float deltaTimeS);

		void cleanUp(bool isBase = false);

	private:

		AnimationType type;

		std::vector<const Texture*>* textures = nullptr;

		float endTime;

		bool loop;

		bool ended = false;

		float currentTime = 0.0f;

		float timeMultiplier = 1.0f;
};

#endif