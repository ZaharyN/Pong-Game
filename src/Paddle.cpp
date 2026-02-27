#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& size, const sf::Vector2f& startPosition, const sf::Color& initialColor, const float& speed)
	: initialSpeed(speed), currentSpeed(speed), horizontalDirection(0)
{
	body = sf::RectangleShape(size);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(startPosition);
	body.setFillColor(initialColor);
}

void Paddle::Update(float deltaT)
{
	
}

void Paddle::Draw(const sf::RenderWindow& window)
{

}

Paddle::~Paddle()
{

}