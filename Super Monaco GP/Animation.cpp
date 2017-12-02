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

bool Animation::hasEnded() const
{
	return ended;
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

float Animation::getTimeMultiplier() const
{
	return timeMultiplier;
}

void Animation::setTimeMultiplier(float timeMultiplier)
{
	assert(timeMultiplier >= 0.0f);

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