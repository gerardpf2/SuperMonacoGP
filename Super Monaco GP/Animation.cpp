#include "Animation.h"

#include "Utils.h"
#include <assert.h>

using namespace std;

Animation::Animation(uint id, float endTime, bool loop, const vector<const Texture*>* textures) :
	id(id), endTime(endTime), loop(loop), textures(textures)
{
	// "textures" is the pointer to the shared data
	// All the animations of the same type will share the same shared data

	reset();
}

Animation::Animation(const Animation& animation) :
	id(animation.id), endTime(animation.endTime), loop(animation.loop), textures(animation.textures)
{
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

bool Animation::hasEnded() const
{
	return ended;
}

uint Animation::getCurrentFrameIndex() const
{
	assert(textures);

	return (uint)(textures->size() * getTimePercent());
}

const Texture* Animation::getCurrentFrame() const
{
	return getFrame(getCurrentFrameIndex());
}

const Texture* Animation::getFrame(uint frameIndex) const
{
	assert(textures);
	assert(textures->size() > frameIndex);

	return textures->at(frameIndex);
}

float Animation::getTimeMultiplier() const
{
	return timeMultiplier;
}

void Animation::setTimeMultiplier(float timeMultiplier)
{
	this->timeMultiplier = timeMultiplier;
}

float Animation::getTimePercent() const
{
	assert(endTime != 0.0f);

	return currentTime / endTime;
}

void Animation::setTimePercent(float timePercent)
{
	currentTime = timePercent * endTime;
}

void Animation::synchronize(const Animation& animation)
{
	setTimePercent(animation.getTimePercent());
}

void Animation::synchronizeInverse(const Animation& animation)
{
	setTimePercent(mod0L(1.0f - animation.getTimePercent(), 1.0f));
}

void Animation::reset()
{
	ended = false;
	currentTime = 0.0f;
	timeMultiplier = 1.0f;
}

void Animation::update(float deltaTimeS)
{
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