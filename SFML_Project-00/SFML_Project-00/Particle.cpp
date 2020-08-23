#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector3f color) :
	m_Position(pos), m_Color(color)
{
	m_Velocity = sf::Vector2f(0.05f, 0.01f);
}

Particle::~Particle()
{

}

void Particle::Update(const sf::Time& deltaTime)
{
	m_Position += m_Velocity * deltaTime.asSeconds();
}

void Particle::ChangeVelocity(const sf::Vector2f velocity)
{
	m_Velocity = velocity;
}
