#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"
#include "Params.h"

#define MAX_PARTICLES_X 1000
#define MAX_PARTICLES_Y 1000

#define PARTICLE_COUNT MAX_PARTICLES_X * MAX_PARTICLES_Y

#define VERTEX_CHUNK PARTICLE_COUNT / 4

void Update(Params p)
{
	sf::Clock clock;
	sf::Time deltaTime = sf::Time::Zero;

	while (p.window->isOpen())
	{
		deltaTime = clock.restart();

		for (int i = 0; i < VERTEX_CHUNK * p.index; i++)
		{
			p.particles[i].Update(deltaTime);

			p.vertices[i].x = p.particles[i].GetPosition().x;
			p.vertices[i].y = p.particles[i].GetPosition().y;
		}
	}
}

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "OpenGL");

	window.setFramerateLimit(60);
	window.setActive(true);

	Particle* particles = new Particle[PARTICLE_COUNT];
	Vertex* vertices = new Vertex[PARTICLE_COUNT];

	bool mousePressed = false;

	float size = PARTICLE_COUNT;
	for (int x = 0; x < MAX_PARTICLES_X; x++)
	{
		for (int y = 0; y < MAX_PARTICLES_Y; y++)
		{
			int i = y * MAX_PARTICLES_X + x;

			sf::Vector2f pos = sf::Vector2f(((float)x / window.getSize().x) - 1.0f, 1.0f - ((float)y / window.getSize().y));
			sf::Vector3f color = sf::Vector3f(1.0f, 0.0f, (float)(x * y) / size);

			particles[i] = Particle(pos, color);

			vertices[i].x = pos.x;
			vertices[i].y = pos.y;

			vertices[i].r = color.x;
			vertices[i].g = color.y;
			vertices[i].b = color.z;
		}
	}

	Params p00 = { &window, particles, vertices, 1 };
	Params p01 = { &window, particles, vertices, 2 };
	Params p02 = { &window, particles, vertices, 3 };
	Params p03 = { &window, particles, vertices, 4 };

	sf::Thread thread00(&Update, p00);
	sf::Thread thread01(&Update, p01);
	sf::Thread thread02(&Update, p02);
	sf::Thread thread03(&Update, p03);

	thread00.launch();
	thread01.launch();
	thread02.launch();
	thread03.launch();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	while (window.isOpen())
	{
		mousePressed = false;

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
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						for (int i = 0; i < PARTICLE_COUNT; i++)
						{
							particles[i].ChangeVelocity(sf::Vector2f(0.01f, -0.05f));
						}
					}
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix();

		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), vertices);
		glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].r);

		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		glPopMatrix();

		window.display();
	}

	return 0;
}