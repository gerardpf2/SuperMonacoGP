#ifndef _MODULE_JSON_
#define _MODULE_JSON_

#include "Module.h"
#include "rapidjson/document.h"

using namespace rapidjson;

class ModuleJson : public Module
{
	public:

		ModuleJson(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleJson();

		void read(const char* path, Document& document) const;
};

#endif