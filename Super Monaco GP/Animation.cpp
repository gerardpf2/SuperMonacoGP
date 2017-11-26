#include "Animation.h"

#include "Utils.h"
#include "ModuleTexture.h"

using namespace std;
using namespace rapidjson;

Animation::Animation(const Animation& animation) :
	type(animation.type), textures(animation.textures), endTime(animation.endTime), loop(animation.loop)
{
	reset();
}

Animation::Animation(const Value& value, const ModuleTexture* moduleTexture)
{
	type = (AnimationType)value["type"].GetUint();
	endTime = value["endTime"].GetFloat();
	loop = value["loop"].GetBool();

	const Value& texturesJson = value["textures"];
	textures = new vector<const Texture*>(texturesJson.Size(), nullptr);

	for(SizeType i = 0; i < texturesJson.Size(); ++i)
		(*textures)[i] = moduleTexture->getTexture((TextureType)texturesJson[i].GetUint());

	reset();
}

Animation::~Animation()
{ }

AnimationType Animation::getType() const
{
	return type;
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
	float newTime = currentTime + timeMultiplier * deltaTimeS;

	if((ended = ended || newTime >= endTime) && !loop) return;

	currentTime = modF0ToL(newTime, endTime);
}

void Animation::cleanUp(bool isBase)
{
	if(isBase)
	{
		textures->clear();
		delete textures;
	}

	textures = nullptr;
}