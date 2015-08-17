#pragma once

#include <MatrixEngine/Window.h>
#include <MatrixEngine/GLSLProgram.h>
#include <MatrixEngine/Camera2D.h>
#include <MatrixEngine/InputManager.h>
#include <MatrixEngine/SpriteBatch.h>
#include <MatrixEngine/Timing.h>

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Member Variables
    MatrixEngine::Window _window; ///< The game window
    
    MatrixEngine::GLSLProgram _textureProgram; ///< The shader program

    MatrixEngine::InputManager _inputManager; ///< Handles input

    MatrixEngine::Camera2D _camera; ///< Main Camera
	MatrixEngine::SpriteBatch _spriteBatch;
	float _time;
	int _fps;
	MatrixEngine::FpsLimiter _fpsLimiter;
	int _screenWidth;
	int _screenHeight;
	float _maxFPS;
};

