#ifndef _MODULE_JSON_
#define _MODULE_JSON_

#include "Module.h"
#include "rapidjson/document.h"

class ModuleJson : public Module
{
	public:

		ModuleJson(GameEngine* gameEngine);

		virtual ~ModuleJson();

		void read(const char* path, rapidjson::Document& document) const;
};

#endif