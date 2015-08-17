#pragma once
#include <glm/glm.hpp>
#include <MatrixEngine/SpriteBatch.h>
#include <random>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 direction, float speed, int lifetime, float rollX, float rollY);
	~Bullet();

	void draw(MatrixEngine::SpriteBatch& spriteBatch);
	// Returns true when bullet is out of lifetime
	bool update();

private:
	int _lifetime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

