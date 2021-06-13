#include "Camera.h"

Camera::Camera(const sf::Window& window) : window(window)
{
	position = (sf::Vector2f)window.getSize() / 2.0f;
	scale = 1.0f;
}

void Camera::update(const InputHandler& inputHandler)
{
	if (inputHandler.get_key_pressed(sf::Keyboard::Key::Space))
	{
		position = (sf::Vector2f)window.getSize() / 2.0f;
		scale = 1.0f;
	}

	if (inputHandler.get_middle_pressed())
		dragPos = get_mouse_world_position();
	if (inputHandler.get_middle_held())
		position += (sf::Vector2f)(dragPos - get_mouse_world_position());

	if (inputHandler.get_scroll_down())
		scale *= 0.85f;
	if (inputHandler.get_scroll_up())
		scale *= 1.15f;
}
