#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GLU.h>
#include <iostream>

class Camera
{
public:
	Camera() = delete;
	Camera(const sf::Window& window);

	void poll_event(const sf::Event& event);

	/// <summary>
	/// does not give accurate coordinates...
	/// </summary>
	template<typename T>
	sf::Vector2<T> view_to_world(const sf::Vector2<T>& position) const;

private:
	void key_pressed(const sf::Event& event);

	void mouse_moved(const sf::Event& event);
	void mouse_wheel_scrolled(const sf::Event& event);
	void mouse_button_pressed(const sf::Event& event);
	void mouse_button_released(const sf::Event& event);

public:
	inline const float* world_matrix() const
	{
		sf::Transform world_matrix;
		world_matrix = world_matrix
			.translate((sf::Vector2f)window.getSize() / 2.0f)
			.scale(scale, scale)
			.translate(-position);

		return world_matrix.getMatrix();
	}
	inline sf::Transform view_matrix() const
	{
		sf::Transform view_matrix;
		view_matrix = view_matrix
			.translate(position)
			.scale(1.0f / scale, 1.0f / scale)
			.translate((sf::Vector2f)window.getSize() / -2.0f);

		return view_matrix;
	}

	inline sf::Vector2f get_position() const { return position; }
	inline sf::Vector2i get_mouse_world_position() const { return view_to_world(mouse.getPosition(window)); }

	inline double get_scale()	 const { return scale; }
	inline bool get_left_hold()	 const { return leftHold; }
	inline bool get_right_hold() const { return rightHold; }

private:
	const sf::Window& window;

	// Camera
	sf::Vector2f position;
	bool moveCamera;
	bool leftHold, rightHold;
	float scale;

	// Mouse
	sf::Mouse mouse;
	sf::Vector2i mousePos;
	sf::Vector2i dragPos;
};

