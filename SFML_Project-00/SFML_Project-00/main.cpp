#include <iostream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"
#include "Params.h"

#define MAX_PARTICLES_X 500
#define MAX_PARTICLES_Y 500

#define PARTICLE_COUNT MAX_PARTICLES_X * MAX_PARTICLES_Y

#define VERTEX_CHUNK PARTICLE_COUNT / 4

void Update(Params p)
{
	sf::Clock clock;
	float deltaTime = FLT_EPSILON;

	while (p.window->isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		for (int i = 0; i < VERTEX_CHUNK * p.index; i++)
		{
			p.particles[i].Update(p.window, deltaTime);
		}
	}
}

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "OpenGL");

	window.setFramerateLimit(60);
	window.setActive(true);

	sf::Mouse mouse;
	sf::Vector2i mousePos;

	bool mousePressed = false;

	Particle* particles = new Particle[PARTICLE_COUNT];
	Vertex* vertices = new Vertex[PARTICLE_COUNT];

	float size = PARTICLE_COUNT;
	for (int x = 0; x < MAX_PARTICLES_X; x++)
	{
		for (int y = 0; y < MAX_PARTICLES_Y; y++)
		{
			int i = y * MAX_PARTICLES_X + x;

			sf::Vector2f pos = sf::Vector2f((float)x, (float)y);
			sf::Vector3f color = sf::Vector3f(1.0f, 0.0f, (float)(x * y) / size);

			particles[i] = Particle(pos, color);

			vertices[i].x = pos.x;
			vertices[i].y = pos.y;

			vertices[i].r = color.x;
			vertices[i].g = color.y;
			vertices[i].b = color.z;
		}
	}

	Params p00 = { &window, particles, 1 };
	Params p01 = { &window, particles, 2 };
	Params p02 = { &window, particles, 3 };
	Params p03 = { &window, particles, 4 };

	sf::Thread thread00(&Update, p00);
	sf::Thread thread01(&Update, p01);
	sf::Thread thread02(&Update, p02);
	sf::Thread thread03(&Update, p03);

	thread00.launch();
	thread01.launch();
	thread02.launch();
	thread03.launch();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window.getSize().x, 0, window.getSize().y);
	glScalef(1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	while (window.isOpen())
	{
		mousePressed = false;
		mousePos = mouse.getPosition(window);

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
						mousePressed = true;
					}
					break;
			}
		}

		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			if (mousePressed)
			{
				particles[i].MoveToMouse(sf::Vector2f(mousePos.x, mousePos.y));
			}

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
	}

	return 0;
}