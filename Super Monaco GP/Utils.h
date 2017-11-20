#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include "Globals.h"

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

static float ease(float a, float b, float percent)
{
	return a + (b - a) * ((-cos(percent * PI) / 2.0f) + 0.5f);
}

static float linear(float x, float x0, float x1, float y0, float y1)
{
	return y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);
}

static unsigned int rgbaToUIint(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	return a << 24 | b << 16 | g << 8 | r;
}

#endif