#include "Particle.h"

Particle::Particle()
{
	m_Velocity = sf::Vector2f();
	m_Acceleration = sf::Vector2f();
	m_Friction = 0.1f;
	m_Mass = 1.0f;
}

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f();
	m_Acceleration = sf::Vector2f();
	m_Friction = 0.1f;
	m_Mass = 1.0f;
}

Particle::~Particle()
{

}

void Particle::Update(const sf::Window& window, float deltaTime)
{
	m_Velocity += m_Acceleration * deltaTime - m_Friction * m_Velocity;

	const sf::Vector2f oldPosition = m_Position;
	m_Position += m_Velocity * deltaTime;

#if WALL_COLLISION
	WallCollision(window, oldPosition);
#endif

	m_Acceleration += -m_Friction * m_Velocity;

	m_Color = sf::Vector3f((m_Position.x * m_Position.y) / (window.getSize().x * window.getSize().y), 0.05f, 0.40f);
}

void Particle::WallCollision(const sf::Window& window, const sf::Vector2f& oldPos)
{
	if (m_Position.x < 0.0f || m_Position.x > window.getSize().x)
	{
		m_Position.x = oldPos.x;

		m_Velocity.x *= -1;
		m_Acceleration.x *= -1;
	}
	if (m_Position.y < 0.0f || m_Position.y > window.getSize().y)
	{
		m_Position.y = oldPos.y;

		m_Velocity.y *= -1;
		m_Acceleration.y *= -1;
	}
}
