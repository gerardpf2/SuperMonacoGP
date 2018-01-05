#ifndef _MINIMAP_UI_
#define _MINIMAP_UI_

#include "Globals.h"
#include <SDL_rect.h>

#define COURSE_RECT_SCALE (1.25f * WINDOW_SCALE)
#define COURSE_RECT_X (int)(490 * WINDOW_SCALE)
#define COURSE_RECT_Y (int)(140 * WINDOW_SCALE)
#define COURSE_RECT_W (int)(112 * COURSE_RECT_SCALE)
#define COURSE_RECT_H (int)(176 * COURSE_RECT_SCALE)
#define COURSE_RECT SDL_Rect{ COURSE_RECT_X, COURSE_RECT_Y, COURSE_RECT_W, COURSE_RECT_H }

#define PLAYER_ICON_W (int)(0.8f * 12 * WINDOW_SCALE)
#define PLAYER_ICON_H (int)(0.8f * 14 * WINDOW_SCALE)

#define ENEMY_CAR_ICON_W (int)(0.8f * 10 * WINDOW_SCALE)
#define ENEMY_CAR_ICON_H (int)(0.8f * 12 * WINDOW_SCALE)

#endif