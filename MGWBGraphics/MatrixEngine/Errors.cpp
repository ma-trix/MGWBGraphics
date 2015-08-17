#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Errors.h"
namespace MatrixEngine
{
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(-1);
	}
}