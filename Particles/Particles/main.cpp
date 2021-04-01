#include <iostream>
#include <execution>
#include <functional>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"
#include "Camera.h"
#include "Utilities.h"
#include "Vector2.h"

const size_t MAX_PARTICLES_X = 1400;
const size_t MAX_PARTICLES_Y = 1425;

const size_t PARTICLE_COUNT = MAX_PARTICLES_X * MAX_PARTICLES_Y;

int main()
{
	sf::Window window(sf::VideoMode(2240, 1260), "Particles");
	window.setActive(true);

	sf::Clock clock;
	float deltaTime = FLT_EPSILON;

	int applyForce = 0;

	Camera* camera = new Camera(window);
	sf::Vector2i mousePos;

	Particle* particles = new Particle[PARTICLE_COUNT];
	Vertex* vertices = new Vertex[PARTICLE_COUNT];
	Color* colors = new Color[PARTICLE_COUNT];

	float size = PARTICLE_COUNT;
	for (int y = 0; y < MAX_PARTICLES_Y; ++y)
	{
		for (int x = 0; x < MAX_PARTICLES_Y; ++x)
		{
			int i = x + y * MAX_PARTICLES_X;

			sf::Vector2f pos = sf::Vector2f(x % window.getSize().x, y % window.getSize().y);
			sf::Vector3f color = sf::Vector3f(0.1f + ((float)(x * y) / size), 0.0f, 0.0f);

			particles[i] = Particle(pos, color);

			colors[i] = *(Color*)(&particles[i].GetColor());
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	gluOrtho2D(0, window.getSize().x, 0, window.getSize().y);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(2.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

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
				{
					glViewport(0, 0, window.getSize().x, window.getSize().y);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glScalef(1.0f, -1.0f, 1.0f);
					gluOrtho2D(0, window.getSize().x, 0, window.getSize().y);
					glMatrixMode(GL_MODELVIEW);
				} 
				break;
			}

			camera->poll_event(event);
		}

		mousePos = camera->get_mouse_world_position();

		applyForce = 0;
		if (camera->get_left_hold())
			applyForce = 1;
		if (camera->get_right_hold())
			applyForce = -1;

		std::for_each(
			std::execution::par_unseq,
			particles,
			particles + PARTICLE_COUNT,
			[&](Particle& particle)
			{
				if (applyForce != 0)
				{
					const sf::Vector2f direction = v2f::direction((sf::Vector2f)mousePos, particle.GetPosition());
					particle.ApplyForce(v2f::normalize(direction) * 150.0f * (float)applyForce);
				}

				particle.Update(&window, deltaTime);
			});

		for (int i = 0; i < PARTICLE_COUNT; ++i)
			vertices[i] = *(Vertex*)(&particles[i].GetPosition());

		glClear(GL_COLOR_BUFFER_BIT);

		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), vertices);
		glColorPointer(3, GL_FLOAT, sizeof(Color), colors);

		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(camera->world_matrix());

		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		glPopMatrix();

		window.display();
	}

	delete camera;
	delete[] particles;
	delete[] vertices;
	delete[] colors;

	return 0;
}