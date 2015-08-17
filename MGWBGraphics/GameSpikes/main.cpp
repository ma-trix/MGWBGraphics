#include <iostream>
#include "MainGame.h"
#include <random>
#include <chrono>

int main(int argc, char** argv)
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 rng(seed);
	MainGame mainGame;
	mainGame.setRng(rng);
	mainGame.run();

	return 0;
}