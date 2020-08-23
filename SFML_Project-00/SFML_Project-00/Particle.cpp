#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f(0.05f, 0.01f);
}

Particle::~Particle()
{

}

void Particle::Update(const float& deltaTime, const bool& mousePressed)
{
	m_Position += m_Velocity * deltaTime;

	if (mousePressed)
	{
		m_Velocity = sf::Vector2f(-0.1f, -0.01f);
	}
}

void Particle::Draw() const
{

}
