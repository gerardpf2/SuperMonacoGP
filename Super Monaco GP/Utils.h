#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include "Types.h"
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

static float clamp(float v, float min, float max)
{
	return fminf(fmaxf(v, min), max);
}

static float ease(float a, float b, float p)
{
	return a + (b - a) * ((-cosf(p * PI) / 2.0f) + 0.5f);
}

static float interpolate(float x, float x0, float x1, float y0, float y1)
{
	return y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);
}

static uint RGBAToUint(uint r, uint g, uint b, uint a)
{
	return a << 24 | b << 16 | g << 8 | r;
}

static bool isOutsideWindowX(const WindowTrapezoid& windowTrapezoid)
{
	return (windowTrapezoid.nr.x <= 0 && windowTrapezoid.fr.x <= 0) || (windowTrapezoid.nl.x >= WINDOW_WIDTH && windowTrapezoid.fl.x >= WINDOW_WIDTH);
}

#endif