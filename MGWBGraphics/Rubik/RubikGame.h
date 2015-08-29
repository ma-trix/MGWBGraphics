#pragma once
#include <MatrixEngine/GLSLProgram.h>
#include <MatrixEngine/Window.h>
#include <MatrixEngine/SpriteBatch.h>
#include <MatrixEngine/Timing.h>
#include <MatrixEngine/InputManager.h>
#include <MatrixEngine/Camera3D.h>
#include <MatrixEngine/Camera2D.h>

enum class GameState { PLAY, EXIT };

class RubikGame
{
public:
	RubikGame();
	~RubikGame();
	void run();
	void initSystems();
	void initShaders();
	void gameLoop();
	void drawGame();
	void processInput();
private:
	MatrixEngine::GLSLProgram _colorProgram;
	MatrixEngine::Window _window;
	int _screenWidth;
	int _screenHeight;
	MatrixEngine::SpriteBatch _spriteBatch;
	MatrixEngine::FpsLimiter _fpsLimiter;
	int _maxFPS;
	GameState _gameState;
	float _time;
	MatrixEngine::Camera3D _camera;
	MatrixEngine::Camera2D _camera2D;
	float _fps;
	MatrixEngine::InputManager _inputManager;
	GLuint ibo_cube_elements;
	GLuint vbo_cube_vertices;
	GLuint vbo_cube_colors;
	glm::vec2 _savedMouseCoords;
};