#pragma once

namespace MatrixEngine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float targetFPS);
		void setMaxFPS(float targetFPS);
		void begin();
		float end();
	private:
		void calculateFPS();
		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;
	};
}