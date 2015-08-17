#include "Bullet.h"
#include <MatrixEngine/GLTexture.h>
#include <MatrixEngine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>


Bullet::Bullet(glm::vec2 pos, glm::vec2 direction, float speed, int lifetime, float rollX, float rollY)
{
	_lifetime = lifetime;
	_position = pos;
	_direction = glm::normalize(direction);
	// Randomize direction by a small factor
	_direction = glm::rotate(_direction, rollX);
	_speed = speed;
}

Bullet::~Bullet()
{
}

void Bullet::draw(MatrixEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static MatrixEngine::GLTexture texture = MatrixEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	MatrixEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 10.0f, 10.0f);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{
	_position += _direction * _speed;
	_lifetime--;
	if (_lifetime == 0)
	{
		return true;
	}
	return false;
}