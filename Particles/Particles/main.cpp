#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"
#include "Camera.h"
#include "Utilities.h"
#include "Vector2.h"

const size_t MAX_PARTICLES_X = 1500;
const size_t MAX_PARTICLES_Y = 1300;

const size_t PARTICLE_COUNT = MAX_PARTICLES_X * MAX_PARTICLES_Y;

const size_t THREAD_COUNT = 6;
const size_t PARTICLE_CHUNK = PARTICLE_COUNT / THREAD_COUNT;

void Update(sf::Window* window, Particle* particles, sf::Vector2i* mousePos, int* applyForce, size_t index)
{
	sf::Clock clock;

	float deltaTime = FLT_EPSILON;

	while (window->isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		for (size_t i = (index * PARTICLE_CHUNK); i < ((index + 1) * PARTICLE_CHUNK); ++i)
		{
			particles[i].ApplyForce(
				Vec2f::normalize(Vec2f::direction((sf::Vector2f)*mousePos, particles[i].GetPosition())) * 100.0f * (float)(*applyForce));

			particles[i].Update(window, deltaTime);
		}
	}
}

int main()
{
	sf::Window window(sf::VideoMode(2240, 1260), "Particles");

	window.setFramerateLimit(144);
	window.setActive(true);

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

			sf::Vector2f pos = sf::Vector2f((float)x, (float)y);
			sf::Vector3f color = sf::Vector3f(0.1f + ((float)(x * y) / size), 0.0f, 0.0f);

			particles[i] = Particle(pos, color);
		}
	}

	std::vector<sf::Thread*> threads;

	for (size_t i = 0; i < THREAD_COUNT; ++i)
		threads.push_back(new sf::Thread(std::bind(&Update, &window, particles, &mousePos, &applyForce, i)));

	std::for_each(threads.begin(), threads.end(),
	[](sf::Thread* thread)
	{
		thread->launch();
	});

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	gluOrtho2D(0, window.getSize().x, 0, window.getSize().y);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	while (window.isOpen())
	{
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

			camera->poll_event(event);
		}

		mousePos = camera->get_mouse_world_position();

		applyForce = 0;
		if (camera->get_left_hold())
			applyForce = 1;
		if (camera->get_right_hold())
			applyForce = -1;

		for (int i = 0; i < PARTICLE_COUNT; ++i)
		{
			const sf::Vector2f pos = particles[i].GetPosition();
			const sf::Vector3f col = particles[i].GetColor();

			vertices[i].x = pos.x;
			vertices[i].y = pos.y;

			colors[i].r = col.x;
			colors[i].g = col.y;
			colors[i].b = col.z;
		}

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

	return 0;
}