#ifndef _GLOBALS_
#define _GLOBALS_

#define WINDOW_TITLE "Super Monaco GP"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define CAMERA_FOV 90.0f
#define CAMERA_MAX_X 5.0f
#define CAMERA_MIN_Y 5.0f
#define CAMERA_MAX_Y 15.0f
#define CAMERA_INITIAL_Y (CAMERA_MIN_Y + CAMERA_MAX_Y) / 2.0f

#define SEGMENT_LENGTH 10.0f

#define ROAD_WIDTH 20.0f
#define ROAD_MIN_X -10.0f
#define ROAD_MAX_X ROAD_MIN_X + ROAD_WIDTH

#define PI 3.14159265358979323846f

#define BLACK SDL_Color{ 0, 0, 0, 255 }
#define RED SDL_Color{ 255, 0, 0, 255 }
#define BLUE SDL_Color{ 0, 0, 255, 255 }
#define GREEN SDL_Color{ 0, 255, 0, 255 }
#define WHITE SDL_Color{ 255, 255, 255, 255 }

#include "Tuple.h"

typedef Tuple2<int> Position2i;
typedef Tuple3<float> Position3f;

#endif