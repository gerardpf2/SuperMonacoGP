#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include "Types.h"

class Animation
{
	public:

		Animation(const std::vector<const Texture*>* frames, float endTime);

		virtual ~Animation();

		const Texture* getCurrentFrame() const;

		void reset();

		void update(float deltaTimeS);

	private:

		const std::vector<const Texture*>* frames = nullptr;

		float endTime;

		float currentTime = 0.0f;
};

#endif