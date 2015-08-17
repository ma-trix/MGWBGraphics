#include "MainGame.h"

#include <MatrixEngine/MatrixEngine.h>

#include <SDL/SDL.h>
#include <iostream>
#include <GL/glew.h>
#include <MatrixEngine/ResourceManager.h>

MainGame::MainGame() : _screenWidth(800), _screenHeight(600), _time(0.0f), _maxFPS(60)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() {
}

void MainGame::run() {
	initSystems();
	gameLoop();
}

void MainGame::initSystems() {
	MatrixEngine::init();
	_window.create("Zombie game", _screenWidth, _screenHeight, 0);
	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (1) {
		_fpsLimiter.begin();
		processInput();
		_time += 0.1f;
		_camera.update();
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

void MainGame::processInput() {
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;
	SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
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
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//Set the camera matrix
	GLint pLocation = _textureProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	
	_spriteBatch.begin();

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static MatrixEngine::GLTexture texture = MatrixEngine::ResourceManager::getTexture("Textures/red_bricks.png");
	MatrixEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	_spriteBatch.draw(position, uv, texture.id, 0.0f, color);
	
	_spriteBatch.end();
	_spriteBatch.renderBatch();
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	_textureProgram.unuse();
    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}