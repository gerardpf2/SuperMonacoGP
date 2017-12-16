#ifndef _ANIMATION_GRID_
#define _ANIMATION_GRID_

#include <vector>
#include "Types.h"

class Animation;

class AnimationGrid
{
	public:

		AnimationGrid(Animation* animationCenterV);

		virtual ~AnimationGrid();

		Animation* getCurrentAnimation() const;

		void addLeftV(Animation* animationLeftV);

		void addRightV(Animation* animationRightV);

		void addLeftH(Animation* animationCenterLeftH, Animation* animationLeftCenterH);

		void addRightH(Animation* animationCenterRightH, Animation* animationRightCenterH);

		void advance(float inc0, float inc1, float deltaTimeS);

		void cleanUp();

	private:

		Animation* animationCenterV = nullptr;

		std::vector<Animation*> animationsLeftV;

		std::vector<Animation*> animationsRightV;

		std::vector<std::pair<Animation*, Animation*>> animationsLeftH;

		std::vector<std::pair<Animation*, Animation*>> animationsRightH;

		Animation* currentAnimationV = nullptr;

		Animation* currentAnimationH = nullptr;

		uint currentAnimationHIndex;

		bool toSide, isRight;
};

#endif