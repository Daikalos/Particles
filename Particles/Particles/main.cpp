#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Particle.h"

const size_t MAX_PARTICLES_X = 1500;
const size_t MAX_PARTICLES_Y = 1300;

const size_t PARTICLE_COUNT = MAX_PARTICLES_X * MAX_PARTICLES_Y;

const size_t PARTICLE_CHUNK = PARTICLE_COUNT / 6;

void Update(sf::Window* window, Particle* particles, sf::Vector2f* mousePos, int* force, size_t index)
{
	sf::Clock clock;

	float deltaTime = (1.0f / 60.0f);
	float accumulator = 0.0f;

	while (window->isOpen())
	{
		float newTime = clock.restart().asSeconds();
		accumulator += newTime;

		while (accumulator >= deltaTime)
		{
			for (size_t i = (index * PARTICLE_CHUNK); i < ((index + 1) * PARTICLE_CHUNK); ++i)
			{
				particles[i].ApplyForce(Normalize(Direction(*mousePos, particles[i].GetPosition())) * 100.0f * (float)(*force));

				particles[i].Update(window, deltaTime);
			}

			accumulator -= deltaTime;
		}
	}
}

int main()
{
	sf::Window window(sf::VideoMode(1600, 900), "Particles");

	window.setFramerateLimit(144);
	window.setActive(true);

	sf::Mouse mouse;
	sf::Vector2f mousePos;
	sf::Vector2i mouseOldPos;

	int force = 0;

	bool moveCamera = false;
	float cameraPositionX = 0.0f;
	float cameraPositionY = 0.0f;
	float cameraScale = 1.0f;

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

	sf::Thread thread00(std::bind(&Update, &window, particles, &mousePos, &force, 0));
	sf::Thread thread01(std::bind(&Update, &window, particles, &mousePos, &force, 1));
	sf::Thread thread02(std::bind(&Update, &window, particles, &mousePos, &force, 2));
	sf::Thread thread03(std::bind(&Update, &window, particles, &mousePos, &force, 3));
	sf::Thread thread04(std::bind(&Update, &window, particles, &mousePos, &force, 4));
	sf::Thread thread05(std::bind(&Update, &window, particles, &mousePos, &force, 5));

	thread00.launch();
	thread01.launch();
	thread02.launch();
	thread03.launch();
	thread04.launch();
	thread05.launch();

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
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Space)
					{
						cameraPositionX = 0.0f;
						cameraPositionY = 0.0f;
						cameraScale = 1.0f;
					}
					break;
				case sf::Event::MouseWheelScrolled:
					cameraScale *= (event.mouseWheelScroll.delta == 1) ? 1.15f : 0.85f;
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Middle)
					{
						moveCamera = true;
						mouseOldPos = mouse.getPosition(window);
					}
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						force = 1;
					}
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						force = -1;
					}
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Middle)
					{
						moveCamera = false;
					}
					if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right)
					{
						force = 0;
					}
					break;
				case sf::Event::MouseMoved:
					mousePos = sf::Vector2f(
						(float)mouse.getPosition(window).x - cameraPositionX, 
						(float)mouse.getPosition(window).y - cameraPositionY) / cameraScale;

					if (moveCamera)
					{
						const sf::Vector2i mouseNewPos = mouse.getPosition(window);
						const sf::Vector2i deltaPos = mouseNewPos - mouseOldPos;

						cameraPositionX += deltaPos.x;
						cameraPositionY += deltaPos.y;

						mouseOldPos = mouseNewPos;
					}
					break;
			}
		}

		for (int i = 0; i < PARTICLE_COUNT; i++)
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

		glTranslatef(cameraPositionX, cameraPositionY, 0);
		glScalef(cameraScale, cameraScale, 1.0f);

		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

		glPopMatrix();

		window.display();
	}

	return 0;
}