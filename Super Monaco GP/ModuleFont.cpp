#include "ModuleFont.h"

#include <SDL_rect.h>
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

void ModuleFont::renderText(const string& text, const WindowPosition& position, float scale, bool leftToRight) const
{
	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	SDL_Rect dstRect{ position.x, position.y };

	for(uint i = 0; i < (uint)text.size(); ++i)
	{
		const Texture* texture = characters[text[i]];
		if(!texture) texture = characters[' '];

		dstRect.w = (int)(scale * texture->r->w);
		dstRect.h = (int)(scale * texture->r->h);

		moduleRenderer->renderTexture(texture->t, texture->r, &dstRect, texture->hFlipped);

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