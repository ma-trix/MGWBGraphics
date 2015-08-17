#include "RubikGame.h"
#include <MatrixEngine/MatrixEngine.h>
#include <iostream>
#include <MatrixEngine/GLTexture.h>
#include <MatrixEngine/ResourceManager.h>


RubikGame::RubikGame() : _screenWidth(1600), _screenHeight(1000), _maxFPS(60), _gameState(GameState::PLAY)
{
}


RubikGame::~RubikGame()
{
}


void RubikGame::run()
{
	initSystems();
	gameLoop();
}

void RubikGame::initSystems()
{
	MatrixEngine::init();
	_window.create("Rubik 3D", _screenWidth, _screenHeight, MatrixEngine::WindowFlags::BORDERLESS);
	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
	_camera.init(_screenWidth, _screenHeight);
}

void RubikGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void RubikGame::gameLoop()
{
	while(_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();
		processInput();
		_time += 0.1f;
		_camera.update();
		drawGame();
		_fps = _fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if(frameCounter == 60)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void RubikGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//static MatrixEngine::GLTexture texture = MatrixEngine::ResourceManager::getTexture("Textures/PNG/RedSquare80x80.png");
	static MatrixEngine::GLTexture texture = MatrixEngine::ResourceManager::getTexture("Textures/PNG/HeartAyse800x800.png");

	MatrixEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(position, uv, texture.id, 0.0f, color);

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();
	_window.swapBuffer();
}

void RubikGame::processInput()
{
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		_gameState = GameState::EXIT;
	}
}