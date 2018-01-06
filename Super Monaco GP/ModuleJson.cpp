#include "ModuleJson.h"

#include "rapidjson/filereadstream.h"

using namespace rapidjson;

ModuleJson::ModuleJson(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleJson::~ModuleJson()
{ }

void ModuleJson::read(const char* path, Document& document) const
{
	assert(path);

	FILE* file;
	fopen_s(&file, path, "rb");
	
	char readBuffer[65536];
	FileReadStream fileReadStream(file, readBuffer, sizeof(readBuffer));
	
	document.ParseStream(fileReadStream);
	
	fclose(file);
}