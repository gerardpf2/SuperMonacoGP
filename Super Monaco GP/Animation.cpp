#include "Animation.h"

#include "Utils.h"

using namespace std;

Animation::Animation(uint id, const vector<const Texture*>* textures, float endTime, bool loop /* , bool inverse */) :
	id(id), textures(textures), endTime(endTime), loop(loop) /* , inverse(inverse) */
{
	// if(inverse) currentTime = (1.0f - 1.0f / (0.5f + textures->size())) * endTime;
}

Animation::~Animation()
{ }

uint Animation::getId() const
{
	return id;
}

const vector<const Texture*>* Animation::getTextures() const
{
	return textures;
}

float Animation::getEndTime() const
{
	return endTime;
}

bool Animation::getLoop() const
{
	return loop;
}

/* bool Animation::getInverse() const
{
	return inverse;
} */

bool Animation::hasEnded() const
{
	return ended;
}

const Texture* Animation::getCurrentFrame() const
{
	uint index = (uint)(textures->size() * (currentTime / endTime));
	return (*textures)[index];
}

float Animation::getTimeMultiplier() const
{
	return timeMultiplier;
}

void Animation::setTimeMultiplier(float timeMultiplier)
{
	this->timeMultiplier = timeMultiplier;
}

void Animation::reset()
{
	ended = false;
	currentTime = 0.0f;
	timeMultiplier = 1.0f;
}

void Animation::update(float deltaTimeS)
{
	// float inverseMultiplier = inverse ? -1.0f : 1.0f;

	float newTime = currentTime + /* inverseMultiplier * */ timeMultiplier * deltaTimeS;

	if((ended = ended || newTime >= endTime) && !loop) return;
	// if((ended = ended || (!inverse && newTime >= endTime) || (inverse && newTime <= 0.0f)) && !loop) return;

	currentTime = modF0ToL(newTime, endTime);
}

void Animation::cleanUp()
{
	textures = nullptr;
}