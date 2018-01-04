#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include "Types.h"

class Animation
{
	public:

		Animation(uint id, float endTime, bool loop, const std::vector<const Texture*>* textures);

		Animation(const Animation& animation);

		virtual ~Animation();

		uint getId() const;

		float getEndTime() const;

		bool getLoop() const;

		const std::vector<const Texture*>* getTextures() const;

		bool hasEnded() const;

		uint getCurrentFrameIndex() const;

		const Texture* getCurrentFrame() const;

		const Texture* getFrame(uint frameIndex) const;

		float getTimeMultiplier() const;

		void setTimeMultiplier(float timeMultiplier);

		float getTimePercent() const;

		void setTimePercent(float timePercent);

		void synchronize(const Animation& animation);

		void synchronizeInverse(const Animation& animation);

		void reset();

		void update(float deltaTimeS);

		void cleanUp();

	private:

		uint id; // Constructor

		float endTime; // Constructor

		bool loop; // Constructor

		const std::vector<const Texture*>* textures = nullptr; // Constructor

		bool ended = false;

		float currentTime = 0.0f;

		float timeMultiplier = 1.0f;
};

#endif