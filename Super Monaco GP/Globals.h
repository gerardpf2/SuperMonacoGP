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

// 0XAABBGGRR
#define RED 0xFF0000FF
#define BLUE 0xFFFF0000
#define GREEN 0xFF00FF00
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

#include "Tuple.h"

typedef Tuple2<short> Position2s;
typedef Tuple3<float> Position3f;

#include <math.h>

static float degToRad(float d)
{
	return d * PI / 180.0f;
}

static int modI0ToL(int v, int l)
{
	if((v %= l) < 0) v += l;
	return v;
}

static float modF0ToL(float v, float l)
{
	if((v = fmodf(v, l)) < 0.0f) v += l;
	return v;
}

#endif