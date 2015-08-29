#include "InputManager.h"

namespace MatrixEngine
{
	InputManager::InputManager() : _mouseCoords(0.0f, 0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	glm::vec2 InputManager::getMouseCoords()
	{
		return _mouseCoords;
	}

	void InputManager::mouseWheelMotion(int motion)
	{
		_mouseWheelMotion = motion;
	}

	bool InputManager::wasWheelMoved()
	{
		if (_mouseWheelMotion != 0)
		{
			return true;
		}
		return false;
	}

	int InputManager::getMouseWheelMotion()
	{
		int value = _mouseWheelMotion;
		_mouseWheelMotion = 0;
		return value;
	}

	void InputManager::saveMouseLocation()
	{
		_savedMouseCoords = _mouseCoords;
	}

	void InputManager::arcBallOn()
	{
		saveMouseLocation();
		_arcBall = true;
	}

	void InputManager::arcBallOff()
	{
		_arcBall = false;
	}

	bool InputManager::isArcBallON()
	{
		return _arcBall;
	}
}
