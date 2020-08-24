#pragma once

#include <SFML/Graphics.hpp>

inline float Length(const sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline sf::Vector2f Normalize(sf::Vector2f vector)
{
	if (Length(vector) == 0) return vector;

	vector.x *= (1.0f / Length(vector));
	vector.y *= (1.0f / Length(vector));

	return vector;
}

inline sf::Vector2f Direction(sf::Vector2f fromVector, sf::Vector2f toVector)
{
	return sf::Vector2f(fromVector.x - toVector.x, fromVector.y - toVector.y);
}
