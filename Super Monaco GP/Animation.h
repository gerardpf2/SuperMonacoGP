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

		const Texture* getCurrentFrame() const;

		float getTimeMultiplier() const;

		void setTimeMultiplier(float timeMultiplier);

		void reset();

		void update(float deltaTimeS);

		void cleanUp();

	private:

		uint id; // Constructor

		float endTime; // Constructor

		bool loop; // Constructor

		const std::vector<const Texture*>* textures = nullptr; // Constructor

		bool ended;

		float currentTime;

		float timeMultiplier;
};

#endif