#ifndef _TYPES_
#define _TYPES_

#include "Tuple.h"

typedef Tuple2<float, float> Size;
typedef Tuple2<short, short> ScreenPosition;
typedef Tuple3<float, float, float> WorldPosition;

struct SDL_Rect;
struct SDL_Texture;

typedef Tuple2<SDL_Texture*, SDL_Rect> TextureInfo;

typedef Tuple5<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> RumbleColors;

#endif