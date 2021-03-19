#include "Particle.h"

Particle::Particle()
{
	m_Velocity = sf::Vector2f();
	m_Friction = 0.99f;
}

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f();
	m_Friction = 0.99f;
}

Particle::~Particle()
{

}

void Particle::Update(const sf::Window* window, const float& deltaTime)
{
	m_Velocity += m_Acceleration * deltaTime;

#if WALL_COLLISION
	if (WallCollision(window))
		return;
#endif

	m_Position += m_Velocity;

	m_Velocity *= m_Friction;
	m_Acceleration *= 0.0f;
}

bool Particle::WallCollision(const sf::Window* window)
{
	if (m_Position.x + m_Velocity.x < 0.0f)
	{
		m_Position.x = 0.0f;
		m_Acceleration.x *= -1;

		return true;
	}
	else if (m_Position.x + m_Velocity.x > window->getSize().x)
	{
		m_Position.x = (float)window->getSize().x;
		m_Acceleration.x *= -1;

		return true;
	}
	else if (m_Position.y + m_Velocity.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Acceleration.y *= -1;

		return true;
	}
	else if (m_Position.y + m_Velocity.y > window->getSize().y)
	{
		m_Position.y = (float)window->getSize().y;
		m_Acceleration.y *= -1;

		return true;
	}

	return false;
}
