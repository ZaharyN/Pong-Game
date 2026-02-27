#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
protected:
	sf::RectangleShape body;
	const float initialSpeed;
	float currentSpeed;
	int horizontalDirection;

	Paddle(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& speed);

	virtual void Update(float deltaT) = 0;

	virtual void Draw(const sf::RenderWindow& window);

	virtual void Reset() = 0;

	~Paddle();

public:
	float getCurrentVelocity() const
	{
		return this->currentSpeed;
	}

	const float getInitialVelocity() const
	{
		return this->initialSpeed;
	}

	sf::RectangleShape getBody() const
	{
		return this->body;
	}
};