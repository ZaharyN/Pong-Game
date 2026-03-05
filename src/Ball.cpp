#include "Ball.h"

Ball::Ball(float initialRadius, const sf::Vector2f& initialPosition,
	float initialSpeed, int minAngle,
	int maxAngle, const sf::Color& color)
	:initialRadius(initialRadius), initialMovementSpeed(initialSpeed),
	currentMovementSpeed(initialMovementSpeed), currentRadius(initialRadius),
	initialMinAngle(minAngle), initialMaxAngle(maxAngle), color(color), rng(std::random_device{}())
{
	body = sf::CircleShape(initialRadius);
	body.setOrigin(body.getGeometricCenter());
	body.setPosition(initialPosition);
	body.setFillColor(color);

	ResetAngle();
}

void Ball::Update(float deltaT)
{
	float ballX = body.getPosition().x;
	float ballY = body.getPosition().y;
	float ballNewX = ballX + deltaT * currentMovementSpeed * horizontalDirection;
	float ballNewY = ballY + deltaT * currentMovementSpeed * verticalDirection;

	std::cout << "Ball position: (X = " << ballNewX << ", Y = " << ballNewY << ")" << std::endl;
	SetPosition({ ballNewX, ballNewY });
}

void Ball::Draw(sf::RenderTarget& target)
{
	target.draw(body);
}

void Ball::Reset()
{
	currentMovementSpeed = initialMovementSpeed;
	currentRadius = initialRadius;
	ResetAngle();
}

void Ball::SwapHorizontalDirection()
{
	horizontalDirection *= -1;
}

void Ball::SwapVerticalDirection()
{
	verticalDirection *= -1;
}

void Ball::IncreaseSpeed()
{
	currentMovementSpeed += currentMovementSpeed * SPEED_MULTIPLIER;
}

void Ball::ApplySpin(float paddleXDirection)
{
	horizontalDirection += paddleXDirection * INFLUENCE;
	verticalDirection *= -1;

	float length = sqrt(pow(horizontalDirection, 2) + pow(verticalDirection, 2));

	horizontalDirection /= length;
	verticalDirection /= length;
}

void Ball::SetPosition(const sf::Vector2f& newPosition)
{
	body.setPosition(newPosition);
}

float Ball::GetCurrentSpeed() const
{
	return this->currentMovementSpeed;
}

float Ball::GetInitialSpeed() const
{
	return this->initialMovementSpeed;
}

float Ball::GetCurrentRadius() const
{
	return this->currentRadius;
}

const sf::CircleShape& Ball::GetBody() const
{
	return this->body;
}

sf::FloatRect Ball::GetGlobalBounds() const
{
	return this->body.getGlobalBounds();
}

sf::FloatRect Ball::GetLocalBounds() const
{
	return this->body.getLocalBounds();
}

void Ball::ResetAngle()
{
	sf::Angle startingAngle = Ball::GenerateRandomStartingAngle(initialMinAngle, initialMaxAngle);
	horizontalDirection = cos(startingAngle.asRadians());
	verticalDirection = sin(startingAngle.asRadians());
}

sf::Angle Ball::GenerateRandomStartingAngle(int min, int max)
{
	std::uniform_int_distribution random(min, max);

	int randomAngle = random(rng);
	sf::Angle angle = sf::degrees(randomAngle);

	return angle;
}