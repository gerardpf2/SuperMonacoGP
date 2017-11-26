#ifndef _TYPES_
#define _TYPES_

struct SDL_Rect;
struct SDL_Texture;

typedef unsigned int uint;

struct Size { float w, h; };

struct Direction { float x, z; };

struct RumbleColors { uint a, b, c, d, e; };

struct Texture { SDL_Texture* t; const SDL_Rect* r; };

struct WindowPosition { short x, y; };

struct WorldPosition { float x, y, z; };

struct WindowTrapezoid { WindowPosition nl, nr, fr, fl; };

struct WorldTrapezoid { WorldPosition nl, nr, fr, fl; };

#endif