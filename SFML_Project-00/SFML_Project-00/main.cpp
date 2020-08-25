#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"

#define MAX_PARTICLES_X 1000
#define MAX_PARTICLES_Y 1000

#define PARTICLE_COUNT MAX_PARTICLES_X * MAX_PARTICLES_Y

#define VERTEX_CHUNK PARTICLE_COUNT / 4

void Update(sf::Window* window, Particle* particles, sf::Vector2f* mousePos, bool* lmb, int index)
{
	sf::Clock clock;

	bool leftClick = false;
	bool rightClick = false;

	float deltaTime = (1.0f / 60.0f);
	float accumulator = 0.0f;

	while (window->isOpen())
	{
		float newTime = clock.restart().asSeconds();
		accumulator += newTime;

		while (accumulator >= deltaTime)
		{
			for (int i = (index * VERTEX_CHUNK); i < ((index + 1) * VERTEX_CHUNK); i++)
			{
				sf::Vector2f pos = particles[i].GetPosition();
				
				particles[i].ApplyForce(Normalize(Direction(*mousePos, pos)) * 600.0f * (float)(*lmb));

				particles[i].Update(window, deltaTime);
			}

			accumulator -= deltaTime;
		}
	}
}

int main()
{
	sf::Window window(sf::VideoMode(1280, 720), "OpenGL");

	window.setFramerateLimit(60);
	window.setActive(true);

	sf::Mouse mouse;
	sf::Vector2f mousePos;
	bool lmb = false;

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
		}
	}

	sf::Thread thread00(std::bind(&Update, &window, particles, &mousePos, &lmb, 0));
	sf::Thread thread01(std::bind(&Update, &window, particles, &mousePos, &lmb, 1));
	sf::Thread thread02(std::bind(&Update, &window, particles, &mousePos, &lmb, 2));
	sf::Thread thread03(std::bind(&Update, &window, particles, &mousePos, &lmb, 3));

	thread00.launch();
	thread01.launch();
	thread02.launch();
	thread03.launch();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	gluOrtho2D(0, window.getSize().x, 0, window.getSize().y);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	while (window.isOpen())
	{
		mousePos = sf::Vector2f((float)mouse.getPosition(window).x, (float)mouse.getPosition(window).y);

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
						lmb = true;
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						lmb = false;
					}
					break;
			}
		}

		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			vertices[i].x = particles[i].GetPosition().x;
			vertices[i].y = particles[i].GetPosition().y;

			vertices[i].r = ((150.0f + vertices[i].y) / window.getSize().y);
			vertices[i].r = particles[i].GetColor().y;
			vertices[i].r = particles[i].GetColor().z;
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