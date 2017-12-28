#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include <string>
#include "Types.h"
#include "Globals.h"
#include <SDL_rect.h>

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
	if(isinf(time)) timeText = "-'--''--";
	else
	{
		uint minutes = (uint)(time / 60.0f);
		uint seconds = (uint)fmodf(time, 60.0f);
		uint mSeconds = (uint)roundf((time - (uint)time) * 100.0f) % 100;

		char buffer[64]; // Cannot control minutes (size 9 works if minutes < 10)
		sprintf_s(buffer, "%.1u'%.2u''%.2u", minutes, seconds, mSeconds);

		timeText = buffer;
	}
}

static bool getRectsEndlessTexture(const Texture* texture, const SDL_Rect& textureRect, const SDL_Rect& renderRect, bool mirror, float offsetX, SDL_Rect& rect0, SDL_Rect& renderRect0, SDL_Rect& rect1, SDL_Rect& renderRect1)
{
	rect0 = textureRect;
	rect0.x += (!mirror ? (int)offsetX : (int)mod0L(offsetX + texture->r->w / 2.0f, (float)texture->r->w));

	if(rect0.x + rect0.w <= texture->r->x + texture->r->w)
	{
		renderRect0 = renderRect;

		return false;
	}
	else
	{
		rect0.w -= rect0.x + rect0.w - (texture->r->x + texture->r->w);

		rect1 = textureRect;

		if(!mirror)
			rect1.w = textureRect.w - rect0.w;
		else
		{
			rect1.x = rect0.x;
			rect1.w = rect0.w;

			rect0.x = textureRect.x;
			rect0.w = textureRect.w - rect1.w;
		}

		renderRect0 = renderRect;
		renderRect0.w = (int)(renderRect0.w * ((float)rect0.w / textureRect.w));

		renderRect1 = renderRect;
		renderRect1.x += renderRect0.w;
		renderRect1.w = renderRect.w - renderRect0.w;

		return true;
	}
}

static void rankStr(uint index, std::string& indexText)
{
	indexText = std::to_string(index);

	switch(index)
	{
		case 1:
			indexText += "ST";

			break;
		case 2:
			indexText += "ND";

			break;
		case 3:
			indexText += "RD";

			break;
		default:
			indexText += "TH";
	}
}

#endif