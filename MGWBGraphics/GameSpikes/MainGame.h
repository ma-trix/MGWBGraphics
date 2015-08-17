#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <MatrixEngine/GLSLProgram.h>
#include <MatrixEngine/GLTexture.h>
#include <MatrixEngine/Sprite.h>
#include <vector>
#include <MatrixEngine/Window.h>
#include <MatrixEngine/Camera2D.h>
#include <MatrixEngine/SpriteBatch.h>
#include <MatrixEngine/InputManager.h>
#include <MatrixEngine/Timing.h>
#include "Bullet.h"
#include <random>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	void setRng(std::mt19937 mersenne_twister_engine);
private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	MatrixEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	MatrixEngine::GLSLProgram _colorProgram;
	MatrixEngine::Camera2D _camera;

	MatrixEngine::SpriteBatch _spriteBatch;

	MatrixEngine::InputManager _inputManager;
	MatrixEngine::FpsLimiter _fpsLimiter;
	
	std::vector<Bullet> _bullets;

	float _maxFPS;
	float _fps;
	float _time;
	std::mt19937 _rng;
	float _accuracy;
};

