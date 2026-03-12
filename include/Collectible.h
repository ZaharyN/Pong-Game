#pragma once
#include "SFML/Graphics.hpp"

class Collectible
{
	sf::Sprite sprite;
	sf::Vector2f position;

	float width;
	float height;

public:
	Collectible(const sf::Vector2f& screenPos, const sf::Texture& texture, float width, float height, const sf::Color& color);

	void Draw(sf::RenderTarget& gameWindow);
	sf::FloatRect GetBounds() const;
};