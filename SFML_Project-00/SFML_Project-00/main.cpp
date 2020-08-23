#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"

#define MAX_PARTICLES_X 1000
#define MAX_PARTICLES_Y 1000

#define PARTICLE_COUNT MAX_PARTICLES_X * MAX_PARTICLES_Y

#define RandomColor sf::Vector3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX)

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "OpenGL");
	window.setActive(true);

	sf::Clock clock;
	sf::Mouse mouse;

	Particle* particles = new Particle[MAX_PARTICLES_X * MAX_PARTICLES_Y];
	Vertex* vertices = new Vertex[MAX_PARTICLES_X * MAX_PARTICLES_Y];

	float deltaTime = FLT_EPSILON;

	bool mousePressed = false;

	for (int x = 0; x < MAX_PARTICLES_X; x++)
	{
		for (int y = 0; y < MAX_PARTICLES_Y; y++)
		{
			sf::Vector2f pos = sf::Vector2f(((float)x / window.getSize().x) - 1.0f, 1.0f - ((float)y / window.getSize().y));
			sf::Vector3f color = RandomColor;

			particles[y * MAX_PARTICLES_X + x] = Particle(pos, color);

			vertices[y * MAX_PARTICLES_X + x].x = pos.x;
			vertices[y * MAX_PARTICLES_X + x].y = pos.y;

			vertices[y * MAX_PARTICLES_X + x].r = color.x;
			vertices[y * MAX_PARTICLES_X + x].g = color.y;
			vertices[y * MAX_PARTICLES_X + x].b = color.z;
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			mousePressed = false;

			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::Resized:
					glViewport(0, 0, event.size.width, event.size.height);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						mousePressed = true;
					}
					break;
			}
		}

		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			particles[i].Update(deltaTime, mousePressed);

			vertices[i].x = particles[i].GetPosition().x;
			vertices[i].y = particles[i].GetPosition().y;
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();

		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), vertices);
		glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].r);

		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		glPopMatrix();

		window.display();

		std::cout << deltaTime << "\n";
	}

	return 0;
}