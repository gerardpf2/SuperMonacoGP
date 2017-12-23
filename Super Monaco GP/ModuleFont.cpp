#include "ModuleFont.h"

#include "ModuleJson.h"
#include "GameEngine.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"

using namespace std;
using namespace rapidjson;

ModuleFont::ModuleFont(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleFont::~ModuleFont()
{ }

void ModuleFont::renderText(const std::string& text, const WindowPosition& position, HAlignment hAlignment, VAlignment vAlignment, float scaleW, float scaleH, Uint8 modR, Uint8 modG, Uint8 modB, Uint8 modA) const
{
	vector<const Texture*> characterTextures;
	getCharacterTextures(text, characterTextures);

	float width, height;
	getTextWidthHeight(characterTextures, scaleW, scaleH, width, height);

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	if(hAlignment == HAlignment::RIGHT) offsetX = width;
	else if(hAlignment == HAlignment::CENTER) offsetX = 0.5f * width;

	if(vAlignment == VAlignment::TOP) offsetY = height;
	else if(vAlignment == VAlignment::CENTER) offsetY = 0.5f * height;

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	SDL_Rect dstRect{ (int)(position.x - offsetX), (int)(position.y - offsetY) };

	for(const Texture* characterTexture : characterTextures)
	{
		dstRect.w = (int)(scaleW * characterTexture->r->w);
		dstRect.h = (int)(scaleH * characterTexture->r->h);

		moduleRenderer->renderTexture(characterTexture->t, characterTexture->r, &dstRect, characterTexture->hFlipped, modR, modG, modB, modA);

		dstRect.x += dstRect.w;
	}
}

bool ModuleFont::setUp()
{
	const char* jsonPath = "Resources/Configurations/Fonts/Font.json";

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	const char* textureGroupPath = jsonDocument["textureGroupPath"].GetString();

	textureGroupId = moduleTexture->load(textureGroupPath);

	characters.resize(256, nullptr);

	const Value& charactersJson = jsonDocument["characters"];

	for(SizeType i = 0; i < charactersJson.Size(); ++i)
	{
		// id, textureId

		uint id = *charactersJson[i]["id"].GetString();
		uint textureId = charactersJson[i]["textureId"].GetUint();

		characters[id] = moduleTexture->get(textureGroupId, textureId);
	}

	return true;
}

void ModuleFont::cleanUp()
{
	getGameEngine()->getModuleTexture()->unload(textureGroupId);

	characters.clear();
}

void ModuleFont::getTextWidthHeight(const vector<const Texture*>& characterTextures, float scaleW, float scaleH, float& width, float& height) const
{
	width = height = 0.0f;

	for(const Texture* characterTexture : characterTextures)
		if(characterTexture)
		{
			width += characterTexture->r->w;
			height = fmaxf(height, (float)characterTexture->r->h);
		}

	width *= scaleW;
	height *= scaleH;
}

void ModuleFont::getCharacterTextures(const string& text, vector<const Texture*>& characterTextures) const
{
	characterTextures.reserve(text.size());

	for(char character : text)
	{
		const Texture* texture = characters[character];
		if(!texture) texture = characters[' '];

		characterTextures.push_back(texture);
	}
}