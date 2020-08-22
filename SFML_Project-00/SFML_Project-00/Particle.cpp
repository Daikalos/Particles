#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector3f color)
	: m_Vertex(Vertex())
{
	m_Vertex.position = pos;
	m_Vertex.color = color;
}

Particle::~Particle()
{

}

void Particle::Update(const float& deltaTime)
{
	m_Vertex.position += sf::Vector2f(0.01f, -0.02f) * deltaTime;
}

void Particle::Draw() const
{

}
