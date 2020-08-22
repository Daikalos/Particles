#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"

#define MAX_PARTICLES_X 1000
#define MAX_PARTICLES_Y 1000

#define RandomColor sf::Vector3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX)

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "OpenGL");
	window.setActive(true);

	sf::Clock clock;

	Particle* particles = new Particle[MAX_PARTICLES_X * MAX_PARTICLES_Y];
	float deltaTime = FLT_EPSILON;

	for (int x = 0; x < MAX_PARTICLES_X; x++)
	{
		for (int y = 0; y < MAX_PARTICLES_Y; y++)
		{
			particles[y * MAX_PARTICLES_X + x] = Particle(
				sf::Vector2f(((float)x / window.getSize().x) - 1.0f, 1.0f - ((float)y / window.getSize().y)),
				RandomColor);
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, event.size.width, event.size.height);
					break;
			}
		}

		for (int i = 0; i < MAX_PARTICLES_X * MAX_PARTICLES_Y; i++)
		{
			particles[i].Update(deltaTime);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_POINTS);

		for (int i = 0; i < MAX_PARTICLES_X * MAX_PARTICLES_Y; i++)
		{
			sf::Vector2f position = particles[i].GetPosition();
			sf::Vector3f color = particles[i].GetColor();

			glVertex2f(position.x, position.y);
			glColor3f(color.x, color.y, color.z);
		}

		glEnd();	

		window.display();
	}

	return 0;
}