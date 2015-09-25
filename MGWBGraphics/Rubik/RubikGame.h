#pragma once
#include <MatrixEngine/GLSLProgram.h>
#include <MatrixEngine/Window.h>
#include <MatrixEngine/SpriteBatch.h>
#include <MatrixEngine/Timing.h>
#include <MatrixEngine/InputManager.h>
#include <MatrixEngine/Camera3D.h>
#include <MatrixEngine/Camera2D.h>
#include <MatrixEngine/Voxel.h>

enum class GameState { PLAY, EXIT };

class RubikGame
{
public:
	RubikGame();
	~RubikGame();
	void run();

private:
	void initSystems();
	void initShaders(const std::string vShaderFilePath, const std::string fShaderFilePath, std::vector<std::string> attributes);
	void initVoxels(glm::vec3 position, glm::vec3 dimensions, const std::vector<std::string> &texPaths);
	void gameLoop();
	void updateCamera();
	void timeProgress();
	void frameSetUp();
	void frameTearDown();
	void drawGame();
	void processInput();
	void prepareP();
	void prepareV();
	void prepareM();
	
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

	glm::mat4 _projection;
	glm::mat4 _view;
	glm::mat4 _model;

	MatrixEngine::Voxel _voxel;
	MatrixEngine::Voxel _voxel2;

	GLint _pLoc;
	GLint _vLoc;
	GLint _mLoc;
	GLint _texLoc;
};