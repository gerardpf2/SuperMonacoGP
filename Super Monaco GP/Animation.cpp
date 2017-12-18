#include "Animation.h"

#include "Utils.h"
#include <assert.h>

using namespace std;

Animation::Animation(uint id, float endTime, bool loop, const vector<const Texture*>* textures) :
	id(id), endTime(endTime), loop(loop), textures(textures)
{
	assert(endTime > 0.0f);
	assert(textures);
	assert(textures->size() > 0);

	// "textures" is the pointer to the shared data
	// All the animations of the same type will share the same shared data

	reset();
}

Animation::Animation(const Animation& animation) :
	id(animation.id), endTime(animation.endTime), loop(animation.loop), textures(animation.textures)
{
	assert(endTime > 0.0f);
	assert(textures);
	assert(textures->size() > 0);

	// "textures" is the pointer to the shared data
	// All the animations of the same type will share the same shared data

	reset();
}

Animation::~Animation()
{ }

uint Animation::getId() const
{
	return id;
}

float Animation::getEndTime() const
{
	return endTime;
}

bool Animation::getLoop() const
{
	return loop;
}

const vector<const Texture*>* Animation::getTextures() const
{
	return textures;
}

float Animation::getTimePercent() const
{
	return currentTime / endTime;
}

bool Animation::hasEnded() const
{
	return ended;
}

uint Animation::getCurrentFrameIndex() const
{
	return (uint)(textures->size() * (currentTime / endTime));
}

const Texture* Animation::getCurrentFrame() const
{
	assert(endTime > 0.0f);
	assert(currentTime >= 0.0f);
	assert(currentTime < endTime);
	assert(textures);
	assert(textures->size() > 0);

	uint index = (uint)(textures->size() * (currentTime / endTime));
	return (*textures)[index];
}

const Texture* Animation::getFrame(uint frameIndex) const
{
	return (*textures)[frameIndex];
}

float Animation::getTimeMultiplier() const
{
	return timeMultiplier;
}

void Animation::setTimeMultiplier(float timeMultiplier)
{
	assert(timeMultiplier >= 0.0f);

	this->timeMultiplier = timeMultiplier;
}

void Animation::synchronize(const Animation& animation)
{
	float percent = animation.currentTime / animation.endTime;
	currentTime = percent * endTime;
}

void Animation::synchronizeInverse(const Animation& animation)
{
	float percent = mod0L((1.0f - animation.currentTime / animation.endTime), 1.0f);
	currentTime = percent * endTime;
}

/* void Animation::advancePercent(float percent)
{
	currentTime += percent * endTime;
	currentTime = mod0L(currentTime, endTime);
} */

void Animation::reset()
{
	ended = false;
	currentTime = 0.0f;
	timeMultiplier = 1.0f;
}

void Animation::update(float deltaTimeS)
{
	assert(endTime > 0.0f);
	assert(currentTime >= 0.0f);
	assert(currentTime < endTime);
	assert(timeMultiplier >= 0.0f);

	float newTime = currentTime + timeMultiplier * deltaTimeS;

	if((ended = ended || newTime >= endTime) && !loop) return;

	currentTime = mod0L(newTime, endTime);
}

void Animation::cleanUp()
{
	// Invalidate "textures"
	// "textures" content will be deleted externally

	textures = nullptr;
}