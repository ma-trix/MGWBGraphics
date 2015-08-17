#include <iostream>
#include <string>
#include "MainGame.h"
#include <MatrixEngine/Errors.h>
#include <MatrixEngine/MatrixEngine.h>
#include <MatrixEngine/ResourceManager.h>

MainGame::MainGame() : 
	_screenWidth(1024), 
	_screenHeight(768), 
	_gameState(GameState::PLAY),
	_time(0.0f),
	_maxFPS(160.0f),
	_accuracy(1.0)
{
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}


void MainGame::run()
{
	initSystems();
	
	gameLoop();
}

void MainGame::setRng(std::mt19937 rng)
{
	_rng = rng;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();

		processInput();
		_time += 0.1f;

		_camera.update();

		for (int i = 0; i < _bullets.size();)
		{
			if(_bullets[i].update())
			{
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else
			{
				i++;
			}
		}

		drawGame();

		_fps = _fpsLimiter.end();
		
		static int frameCoutner = 0;
		frameCoutner++;
		if (frameCoutner == 10000)
		{
			std::cout << _fps << std::endl;
			frameCoutner = 0;
		}
	}
}

void MainGame::processInput()
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
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		
		float accLow = -M_PI * 0.25f * (1 - _accuracy);
		float accHigh = M_PI * 0.25f * (1 - _accuracy);
		std::uniform_real_distribution<float> accRoll(accLow, accHigh);

		_bullets.emplace_back(playerPosition, direction, 5.0f, 300, accRoll(_rng), accRoll(_rng));
	}
	if (_inputManager.isKeyPressed(SDLK_r))
	{
		if (_accuracy < 0.95f) 
		{
			_accuracy += 0.05f;
		}
		else
		{
			_accuracy = 1.0f;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_f))
	{
		if (_accuracy > 0.05f) 
		{
			_accuracy -= 0.05f;
		}
		else
		{
			_accuracy = 0.0f;
		}
	}
}

void MainGame::initSystems()
{
	MatrixEngine::init();
	
	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::drawGame()
{
	//Set the base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader
	_colorProgram.use();

	//We are using texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//Get the uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	glUniform1i(textureLocation, 0);

	//Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();
	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static MatrixEngine::GLTexture texture = MatrixEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterLeft_Standing.png");
	MatrixEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	
	_spriteBatch.draw(position, uv, texture.id, 0.0f, color);
	
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//disable the shader
	_colorProgram.unuse();
	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}