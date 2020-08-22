#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

struct Vertex
{
	sf::Vector2f position;
	sf::Vector3f color;
};

class Particle
{
public:
	Particle() 
	{
		m_Vertex = Vertex();
	}

	Particle(sf::Vector2f pos, sf::Vector3f color);
	~Particle();

	void Update(const float& deltaTime);
	void Draw() const;

	inline Vertex GetVertex() const { return m_Vertex; }

	inline sf::Vector2f GetPosition() const { return m_Vertex.position; }
	inline sf::Vector3f GetColor() const { return m_Vertex.color; }

	inline sf::Vector2f GetVelocity() const { return m_Velocity; }

private:
	Vertex m_Vertex;
	sf::Vector2f m_Velocity;
};

