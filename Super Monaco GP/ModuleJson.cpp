#include "ModuleJson.h"

#include "rapidjson/filereadstream.h"

ModuleJson::ModuleJson(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleJson::~ModuleJson()
{ }

void ModuleJson::read(const char* path, Document& document) const
{
	FILE* file;
	fopen_s(&file, path, "rb");
	
	char readBuffer[65536];
	FileReadStream fileReadStream(file, readBuffer, sizeof(readBuffer));
	
	document.ParseStream(fileReadStream);
	
	fclose(file);
}