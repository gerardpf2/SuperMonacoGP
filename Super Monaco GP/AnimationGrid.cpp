#include "AnimationGrid.h"

#include <assert.h>
#include "Animation.h"

using namespace std;

AnimationGrid::AnimationGrid(Animation* animationCenterV) :
	animationCenterV(animationCenterV)
{
	currentAnimationV = animationCenterV;
}

AnimationGrid::~AnimationGrid()
{ }

Animation* AnimationGrid::getCurrentAnimation() const
{
	return currentAnimationV;
}

void AnimationGrid::addLeftV(Animation* animationLeftV)
{
	animationsLeftV.push_back(animationLeftV);
}

void AnimationGrid::addRightV(Animation* animationRightV)
{
	animationsRightV.push_back(animationRightV);
}

void AnimationGrid::addLeftH(Animation* animationCenterLeftH, Animation* animationLeftCenterH)
{
	animationsLeftH.push_back(pair<Animation*, Animation*>(animationCenterLeftH, animationLeftCenterH));
}

void AnimationGrid::addRightH(Animation* animationCenterRightH, Animation* animationRightCenterH)
{
	animationsRightH.push_back(pair<Animation*, Animation*>(animationCenterRightH, animationRightCenterH));
}

void AnimationGrid::advance(float inc0, float inc1, float deltaTimeS)
{
	/*

	Manage v animation and h animation

	Possible situations:
	- 1) Car center (move forward + wheels centered)
	- 2) Car center to side (move forward + wheels oriented)
	- 3) Car side to center (move forward + wheels oriented)

	1) If car center:
	- Keep center
	- Start move to side

	2) If car center to side:
	- Keep center to side
	- Start move to center (end move to side)

	3) If car side to center:
	- Keep side to center
	- Car center (end move to center)
	- Start move to side (depending on the direction)

	*/

	// 1) Car center
	if(currentAnimationV == animationCenterV)
	{
		// Start move to side, find h animation
		if(inc1 > 0.0f)
		{
			assert(currentAnimationV);

			uint index = (uint)(currentAnimationV->getTimePercent() * animationsRightH.size());
			
			assert(animationsRightH.size() > index);
			
			currentAnimationH = animationsRightH[index].first;

			assert(currentAnimationH);

			currentAnimationH->reset();

			currentAnimationHIndex = index;

			// Center to right side
			toSide = true;
			isRight = true;
		}
		else if(inc1 < 0.0f)
		{
			assert(currentAnimationV);

			uint index = (uint)(currentAnimationV->getTimePercent() * animationsLeftH.size());

			assert(animationsLeftH.size() > index);

			currentAnimationH = animationsLeftH[index].first;

			assert(currentAnimationH);

			currentAnimationH->reset();

			currentAnimationHIndex = index;

			// Center to left side
			toSide = true;
			isRight = false;
		}
	}

	// 1) Car center to side and 2) Car side to center
	if(currentAnimationH)
	{
		// 1) Car center to side
		if(toSide)
		{
			// Start move to center
			if(inc1 == 0.0f || isRight && inc1 < 0.0f || !isRight && inc1 > 0.0f)
			{
				assert(currentAnimationH);
				
				if(currentAnimationH->hasEnded()) currentAnimationH->reset();

				assert(animationsLeftH.size() > currentAnimationHIndex);
				assert(animationsRightH.size() > currentAnimationHIndex);

				Animation* currentAnimationInverseH = isRight ? animationsRightH[currentAnimationHIndex].second : animationsLeftH[currentAnimationHIndex].second;
				
				assert(currentAnimationInverseH);
				
				currentAnimationInverseH->synchronizeInverse(*currentAnimationH);
				currentAnimationH = currentAnimationInverseH;

				toSide = false;
			}
		}
		// 2) Car side to center
		else
		{
			assert(currentAnimationH);

			// Car center, end move
			if(currentAnimationH->hasEnded())
			{
				currentAnimationH->reset();
				currentAnimationH = nullptr;

				currentAnimationV = animationCenterV;
			}
			else
			{
				// Start move to side, moving from center to right side before ending side to center move
				if(isRight && inc1 > 0.0f)
				{
					assert(animationsRightH.size() > currentAnimationHIndex);

					Animation* currentAnimationInverseH = animationsRightH[currentAnimationHIndex].first;

					assert(currentAnimationInverseH);

					currentAnimationInverseH->synchronizeInverse(*currentAnimationH);
					currentAnimationH = currentAnimationInverseH;

					toSide = true;
				}
				// Start move to side, moving from center to left side before ending side to center move
				else if(!isRight && inc1 < 0.0f)
				{
					assert(animationsLeftH.size() > currentAnimationHIndex);

					Animation* currentAnimationInverseH = animationsLeftH[currentAnimationHIndex].first;

					assert(currentAnimationInverseH);

					currentAnimationInverseH->synchronizeInverse(*currentAnimationH);
					currentAnimationH = currentAnimationInverseH;

					toSide = true;
				}
			}
		}

		// If end move (car center), this is null
		if(currentAnimationH)
		{
			currentAnimationH->update(deltaTimeS);

			// Find v animation
			vector<Animation*>& animationsV = isRight ? animationsRightV : animationsLeftV;

			uint index = (uint)(currentAnimationH->getTimePercent() * animationsV.size());
			if(!toSide) index = (uint)animationsV.size() - index - 1;

			assert(animationsV.size() > index);
			assert(animationsV[index]);

			animationsV[index]->synchronize(*currentAnimationV);
			currentAnimationV = animationsV[index];
		}
	}

	assert(currentAnimationV);

	currentAnimationV->setTimeMultiplier(inc0);
	currentAnimationV->update(deltaTimeS);
}

void AnimationGrid::cleanUp()
{
	animationCenterV = nullptr;

	animationsLeftV.clear();

	animationsRightV.clear();

	animationsLeftH.clear();

	animationsRightH.clear();

	currentAnimationV = nullptr;

	currentAnimationH = nullptr;
}