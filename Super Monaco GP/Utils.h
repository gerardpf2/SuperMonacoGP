#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include <string>
#include "Types.h"
#include "Globals.h"

static float degToRad(float d)
{
	return d * PI / 180.0f;
}

static int mod0L(int v, int l)
{
	if((v %= l) < 0) v += l;
	return v;
}

static float mod0L(float v, float l)
{
	if((v = fmodf(v, l)) < 0.0f) v += l;
	return v;
}

static float clamp(float v, float min, float max)
{
	return fminf(fmaxf(v, min), max);
}

static float clamp01(float v)
{
	return fminf(fmaxf(v, 0.0f), 1.0f);
}

static float ease(float a, float b, float p)
{
	return a + (b - a) * ((-cosf(p * PI) / 2.0f) + 0.5f);
}

static float interpolate(float x, float x0, float x1, float y0, float y1)
{
	return y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);
}

static float interpolate01(float x, float x0, float x1)
{
	return interpolate(x, x0, x1, 0.0f, 1.0f);
}

static uint RGBAToUint(uint r, uint g, uint b, uint a)
{
	return a << 24 | b << 16 | g << 8 | r;
}

static bool isOutsideWindowX(const WindowTrapezoid& windowTrapezoid)
{
	return (windowTrapezoid.nr.x <= 0 && windowTrapezoid.fr.x <= 0) || (windowTrapezoid.nl.x >= WINDOW_WIDTH && windowTrapezoid.fl.x >= WINDOW_WIDTH);
}

static uint kmh(float value)
{
	return (uint)interpolate(value, 0.0f, CAR_MAX_VELOCITY, 0.0f, 350.0f);
}

static void time(float time, std::string& timeText)
{
	uint minutes = (uint)(time / 60.0f);
	uint seconds = (uint)fmodf(time, 60.0f);
	uint mSeconds = (uint)roundf((time - (uint)time) * 100.0f) % 100;

	char buffer[64]; // Cannot control minutes (size 9 works if minutes < 10)
	sprintf_s(buffer, "%.1u'%.2u''%.2u", minutes, seconds, mSeconds);

	timeText = buffer;
}

#endif