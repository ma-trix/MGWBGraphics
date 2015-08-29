#include "RubikGame.h"
#include <MatrixEngine/MatrixEngine.h>
#include <iostream>
#include <MatrixEngine/Voxel.h>


RubikGame::RubikGame() : _screenWidth(1600), _screenHeight(900), _maxFPS(60), _gameState(GameState::PLAY)
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
	_window.create("Rubik 3D", _screenWidth, _screenHeight, 0);
	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
	
	float foV = 45.0f;	// Field of view in degrees
	float aspectRatio = 16.0f / 9.0f;	// Aspect ratio width to height
	float front = 1.0f;	// Check http://www.songho.ca/opengl/gl_projectionmatrix.html#comment-1308374035
	float back = 10.0f;	// See above link
	_camera.init(_screenWidth, _screenHeight, foV, aspectRatio, front, back);
	glm::vec3 position{0.0f, 0.0f, 0.0f};
	_camera.setPosition(position);
	_camera.setScale(1.0f);
	
	_camera2D.init(_screenWidth, _screenHeight);
	_camera2D.setPosition(glm::vec2{ 0.0f, 0.0f });
	_camera2D.setScale(1.0f);
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
		_camera2D.update();
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

void drawAxes()
{
	GLuint _vboID = 0;
	GLuint _vao = 0;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 3.0f;
	float h = 3.0f;
	float d = 3.0f;

	if (_vboID == 0)
	{
		glGenBuffers(1, &_vboID);
	}

	MatrixEngine::Vertex3D vertexData[6];

	vertexData[0].setPosition(x, y, z);
	vertexData[1].setPosition(x + w, y, z);

	vertexData[2].setPosition(x, y, z);
	vertexData[3].setPosition(x, y + h, z);

	vertexData[4].setPosition(x, y, z);
	vertexData[5].setPosition(x, y, z + w);

	vertexData[0].setColor(255, 0, 0, 255);
	vertexData[1].setColor(255, 0, 0, 255);
	vertexData[2].setColor(0, 255, 0, 255);
	vertexData[3].setColor(0, 255, 0, 255);
	vertexData[4].setColor(0, 0, 255, 255);
	vertexData[5].setColor(0, 0, 255, 255);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//unbinding buffer, 0 means no buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_vao == 0)
	{
		glGenVertexArrays(1, &_vao);
	}
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, uv));

	glDrawArrays(GL_LINES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void RubikGame::drawGame()
{
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getProjectionMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLint vLocation = _colorProgram.getUniformLocation("V");

	glm::mat4 view = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	glUniformMatrix4fv(vLocation, 1, GL_FALSE, &(view[0][0]));

	GLint mLocation = _colorProgram.getUniformLocation("M");

	glm::mat4 model = _camera.getCameraMatrix();
	glUniformMatrix4fv(mLocation, 1, GL_FALSE, &(model[0][0]));

	auto texture_path1 = "Textures/PNG/Face1Yellow80x80.png";
	auto texture_path2 = "Textures/PNG/Face2Blue80x80.png";
	auto texture_path3 = "Textures/PNG/Face3Green80x80.png";
	auto texture_path4 = "Textures/PNG/Face4Red80x80.png";
	auto texture_path5 = "Textures/PNG/Face5Orange80x80.png";
	auto texture_path6 = "Textures/PNG/Face6White80x80.png";

	MatrixEngine::Voxel voxel;
	voxel.init(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, { texture_path1, texture_path2, texture_path3, texture_path4, texture_path5, texture_path6 });
	voxel.draw();

	MatrixEngine::Voxel voxel2;
	voxel2.init(1.5f, 0.5f, 0.0f, 0.8f, 1.0f, 1.0f, "Textures/PNG/HeartAyse80x80.png");
	voxel2.draw();
	
	drawAxes();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();
	_window.swapBuffer();
}

void RubikGame::processInput()
{
	const float CAMERA_SPEED = 0.5f;
	const float SCALE_SPEED = 0.01f;
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
		case SDL_MOUSEWHEEL:
			_inputManager.mouseWheelMotion(event.wheel.y);
			break;
		}
	}
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec3(0.0f, 0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec3(0.0f, 0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec3(CAMERA_SPEED, 0.0f, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec3(-CAMERA_SPEED, 0.0f, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if(_inputManager.wasWheelMoved())
	{
		_camera.zoom(_inputManager.getMouseWheelMotion());
	}
	if (_inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		_gameState = GameState::EXIT;
	}
}