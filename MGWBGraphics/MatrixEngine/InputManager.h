#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL/SDL_atomic.h>

namespace MatrixEngine 
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();
		
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);
		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords();
		void mouseWheelMotion(int motion);
		bool wasWheelMoved();
		int getMouseWheelMotion();
		void arcBallOn();
		void arcBallOff();
		bool isArcBallON();
	private:
		void saveMouseLocation();
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
		int _mouseWheelMotion;
		glm::vec2 _savedMouseCoords;
		bool _arcBall;
	};
}