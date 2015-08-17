#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

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
	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};
}