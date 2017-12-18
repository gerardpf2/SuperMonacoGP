#ifndef _TYPES_
#define _TYPES_

struct SDL_Rect;
struct SDL_Texture;

class GameObject;

typedef unsigned int uint;

struct Size { float w, h; };

struct Direction { float x, z; };

struct RumbleColors { uint a, b, c, d, e; };

struct Texture { SDL_Texture* t; const SDL_Rect* r; bool hFlipped; };

struct WindowPosition { short x, y; };

struct WorldPosition { float x, y, z; };

struct WindowTrapezoid { WindowPosition nl, nr, fr, fl; };

struct WorldTrapezoid { WorldPosition nl, nr, fr, fl; };

struct RoadGameObjectDefinition { uint id; float offsetX; WorldPosition wp; };

struct RoadBackgroundDefinition { uint textureGroupId, textureId, textureSkyId, textureGroundId; };

struct Layer { uint id; SDL_Texture* t; const SDL_Rect* tr; const SDL_Rect* v; };

struct Box { float w, h, d; };

struct Collider { const GameObject* g; Box b; };

#endif