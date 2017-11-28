#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include "Types.h"

class Animation
{
	public:

		Animation(uint id, const std::vector<const Texture*>* textures, float endTime, bool loop /* , bool inverse */);

		virtual ~Animation();

		uint getId() const;

		const std::vector<const Texture*>* getTextures() const;

		float getEndTime() const;

		bool getLoop() const;

		// bool getInverse() const;

		bool hasEnded() const;

		const Texture* getCurrentFrame() const;

		float getTimeMultiplier() const;

		void setTimeMultiplier(float timeMultiplier);

		void reset();

		void update(float deltaTimeS);

		void cleanUp();

	private:

		uint id;

		const std::vector<const Texture*>* textures = nullptr;

		float endTime;

		bool loop;

		// bool inverse;

		bool ended = false;

		float currentTime = 0.0f;

		float timeMultiplier = 1.0f;
};

#endif