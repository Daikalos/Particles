#include "Particle.h"

Particle::Particle()
{
	m_Velocity = sf::Vector2f();
	m_Friction = 0.5f;
}

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f();
	m_Friction = 0.5f;
}

Particle::~Particle()
{

}

void Particle::Update(const sf::Window* window, const float& deltaTime)
{
	const sf::Vector2f nextPos = m_Velocity * deltaTime;

	if (m_Position.x + nextPos.x < 0.0f)
	{
		m_Position.x = 0.0f;
		m_Velocity.x *= -1;

		return;
	}
	else if (m_Position.x + nextPos.x > window->getSize().x)
	{
		m_Position.x = (float)window->getSize().x;
		m_Velocity.x *= -1;

		return;
	}
	else if (m_Position.y + nextPos.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y *= -1;

		return;
	}
	else if (m_Position.y + nextPos.y > window->getSize().y)
	{
		m_Position.y = (float)window->getSize().y;
		m_Velocity.y *= -1;

		return;
	}

	m_Position += nextPos;
	m_Velocity *= 0.97f;
}