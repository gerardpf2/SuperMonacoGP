#ifndef _GLOBALS_
#define _GLOBALS_

#define WINDOW_TITLE "Super Monaco GP"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define CAMERA_Y 5.0f
#define CAMERA_FOV 90.0f

#define CAMERA_FREE_SPEED_Z 10.0f

#define SEGMENT_LENGTH 0.5f
#define DRAW_DISTANCE 200.0f
#define N_SEGMENTS_DRAW (unsigned int)(DRAW_DISTANCE / SEGMENT_LENGTH)

#define ROAD_WIDTH 20.0f
#define ROAD_MIN_X -10.0f
#define ROAD_MAX_X ROAD_MIN_X + ROAD_WIDTH
#define ROAD_MIN_LENGTH DRAW_DISTANCE

#define PI 3.14159265358979323846f

#include "Tuple.h"

typedef Tuple2<short, short> Position2s;
typedef Tuple3<float, float, float> Position3f;

#endif