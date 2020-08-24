#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f();
}

Particle::~Particle()
{

}

void Particle::Update(const sf::Window* window, const float& deltaTime)
{
	sf::Vector2f nextPos = m_Velocity * deltaTime;

	if (m_Position.x + nextPos.x < 0.0f)
	{
		m_Position.x = 0.0f;
		m_Velocity.x *= -1;
	}
	if (m_Position.x + nextPos.x > window->getSize().x)
	{
		m_Position.x = (float)window->getSize().x;
		m_Velocity.x *= -1;
	}
	if (m_Position.y + nextPos.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y *= -1;
	}
	if (m_Position.y + nextPos.y > window->getSize().y)
	{
		m_Position.y = (float)window->getSize().y;
		m_Velocity.y *= -1;
	}
	
	m_Position += nextPos;
}