#include <SDL/SDL.h>
#include "Timing.h"

namespace MatrixEngine
{
	FpsLimiter::FpsLimiter()
	{
		
	}

	void FpsLimiter::init(float targetFPS)
	{
		setMaxFPS(targetFPS);
	}

	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;
	}

	void FpsLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	float FpsLimiter::end()
	{
		calculateFPS();
		float frameTicks = SDL_GetTicks() - _startTicks;
		//Limit the FPS to the max FPS
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}
		return _fps;
	}

	void FpsLimiter::calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static float prevTicks = SDL_GetTicks();
		static int currentFrame = 0;
		float currentTicks;

		currentTicks = SDL_GetTicks();
		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;

		prevTicks = currentTicks;

		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}
		float frameTimeAverage = 0.0f;
		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			_fps = 1000.0f / frameTimeAverage;
		}
		else
		{
			_fps = 0.0f;
		}
	}
}