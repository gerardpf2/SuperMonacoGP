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

void ModuleFont::renderText(const std::string& text, const WindowPosition& position, Alignment alignment, float scale, Uint8 modR, Uint8 modG, Uint8 modB) const
{
	vector<const Texture*> characterTextures;
	getCharacterTextures(text, characterTextures);

	float offsetX = 0.0f;

	if(alignment != Alignment::LEFT)
	{
		offsetX = -getTextWidth(characterTextures, scale);
		if(alignment == Alignment::CENTER) offsetX *= 0.5f;
	}

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	SDL_Rect dstRect{ (int)(position.x + offsetX), position.y };

	for(const Texture* characterTexture : characterTextures)
	{
		dstRect.w = (int)(scale * characterTexture->r->w);
		dstRect.h = (int)(scale * characterTexture->r->h);

		SDL_SetTextureColorMod(characterTexture->t, modR, modG, modB);

		moduleRenderer->renderTexture(characterTexture->t, characterTexture->r, &dstRect, characterTexture->hFlipped);
		
		SDL_SetTextureColorMod(characterTexture->t, 255, 255, 255);

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

float ModuleFont::getTextWidth(const vector<const Texture*>& characterTextures, float scale) const
{
	float offsetX = 0.0f;

	for(const Texture* characterTexture : characterTextures)
		if(characterTexture) offsetX += characterTexture->r->w;

	return scale * offsetX;
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