#include "Animation.h"

#include "Utils.h"

using namespace std;

Animation::Animation(const vector<const Texture*>* frames, float endTime) :
	frames(frames), endTime(endTime)
{ }

Animation::~Animation()
{ }

const Texture* Animation::getCurrentFrame() const
{
	uint index = (uint)(frames->size() * (currentTime / endTime));
	return (*frames)[index];
}

void Animation::reset()
{
	currentTime = 0.0f;
}

void Animation::update(float deltaTimeS)
{
	currentTime = modF0ToL(currentTime + deltaTimeS, endTime);

	// printf("%.3f . . . %u\n", currentTime, (uint)(frames->size() * (currentTime / endTime)));
}