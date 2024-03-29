#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#define WALL_COLLISION 1

class Particle
{
public:
	Particle();

	Particle(sf::Vector2f pos, sf::Vector3f color);
	~Particle();

	void Update(const sf::Window& window, float deltaTime);

	inline void ApplyForce(const sf::Vector2f& force) 
	{
		m_Acceleration += force / m_Mass;
	}

	inline sf::Vector2f GetPosition() const { return m_Position; }
	inline sf::Vector2f GetVelocity() const { return m_Velocity; }
	inline sf::Vector2f GetAcceleration() const { return m_Acceleration; }

	inline sf::Vector3f GetColor() const { return m_Color; }

private:
	void WallCollision(const sf::Window& window, const sf::Vector2f& oldPos);

private:
	float m_Friction;
	float m_Mass;

	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
	sf::Vector2f m_Acceleration;

	sf::Vector3f m_Color;
};

