#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Utility.h"

struct Vertex
{
	GLfloat x, y;
	GLfloat r, g, b;
};

class Particle
{
public:
	Particle();

	Particle(sf::Vector2f pos, sf::Vector3f color);
	~Particle();

	void Update(const sf::Window* window, const float& deltaTime);

	void MoveToMouse(const sf::Vector2f mousePos) 
	{
		if (Length(Direction(mousePos, m_Position)) < FLT_EPSILON) return;

		m_Velocity += Normalize(Direction(mousePos, m_Position)) * Clamp((1.0f / Length(Direction(mousePos, m_Position))), 1.0f, FLT_MAX) * 100.0f;
	}

	inline sf::Vector2f GetPosition() const { return m_Position; }
	inline sf::Vector3f GetColor() const { return m_Color; }

	inline sf::Vector2f GetVelocity() const { return m_Velocity; }

private:
	float m_Friction;

	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;

	sf::Vector3f m_Color;
};

