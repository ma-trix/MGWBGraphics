#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
namespace MatrixEngine
{
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		int getScreenWidth();
		int getScreenHeight();
		void swapBuffer();
	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};
}

