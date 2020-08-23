#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

struct Vertex
{
	GLfloat x, y;
	GLfloat r, g, b;
};

class Particle
{
public:
	Particle() { }

	Particle(sf::Vector2f pos, sf::Vector3f color);
	~Particle();

	void Update(const sf::Time& deltaTime);

	void ChangeVelocity(const sf::Vector2f velocity);

	inline sf::Vector2f GetPosition() const { return m_Position; }
	inline sf::Vector3f GetColor() const { return m_Color; }

	inline sf::Vector2f GetVelocity() const { return m_Velocity; }

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	sf::Vector3f m_Color;
};

