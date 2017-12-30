#ifndef _MODULE_SEGA_UI_
#define _MODULE_SEGA_UI_

#include "Globals.h"

#define BASE_ALL_RECT_X 0
#define BASE_ALL_RECT_Y 0
#define BASE_ALL_RECT_W WINDOW_WIDTH
#define BASE_ALL_RECT_H WINDOW_HEIGHT
#define BASE_ALL_RECT SDL_Rect{ BASE_ALL_RECT_X, BASE_ALL_RECT_Y, BASE_ALL_RECT_W, BASE_ALL_RECT_H }

#define LOGO_RECT_W (int)(192 * WINDOW_SCALE)
#define LOGO_RECT_H (int)(64 * WINDOW_SCALE)
#define LOGO_RECT_X (int)(0.5f * (WINDOW_WIDTH - LOGO_RECT_W))
#define LOGO_RECT_Y (int)(0.5f * (WINDOW_HEIGHT - LOGO_RECT_H))
#define LOGO_RECT SDL_Rect{ LOGO_RECT_X, LOGO_RECT_Y, LOGO_RECT_W, LOGO_RECT_H }

#endif