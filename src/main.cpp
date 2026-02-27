#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

enum GameState
{
	Menu,
	Playing,
};

sf::Angle generateRandomStartingAngle(const int& min, const int& max)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution random(min, max);

	int randomAngle = random(mt);
	sf::Angle angle = sf::degrees(randomAngle);
	return angle;
}

int main()
{
	const int buttonWidth = 300;
	const int buttonHeight = 100;

	const unsigned int windowWidth = 1200;
	const unsigned int windowHeight = 600;

	const float playerLength = 100.f;
	const float playerHeight = 10.f;

	const float ballRadius = 5.f;
	const int initialMinAngle = 45;
	const float initialMaxAngle = 135;

	const float playerVelocity = 300.f;
	const float ballInitialVelocity = 300.f;

	GameState gameState = GameState::Menu;

	// Sound and Music
	sf::SoundBuffer selectBuffer;
	if (!selectBuffer.loadFromFile("Assets/Audio/UI_select.ogg"))
	{
		std::cout << "Error: Could not find ui_select sound in Assets/Audio folder!" << std::endl;
	}
	
	sf::SoundBuffer hitBuffer;
	if (!hitBuffer.loadFromFile("Assets/Audio/paddle_hit.wav"))
	{
		std::cout << "Error: Could not find paddle_hit sound in Assets/Audio folder!" << std::endl;
	}

	sf::Sound selectSound(selectBuffer);
	sf::Sound paddleHitSound(hitBuffer);

	sf::Music backgroundMusic;
	if (!backgroundMusic.openFromFile("Assets/Audio/background.mp3"))
	{
		std::cout << "Error: Could not find background music in Assets/Audio folder!" << std::endl;
	}

	// UI
	sf::RectangleShape startRectangle({ buttonWidth, buttonHeight });
	startRectangle.setOrigin(startRectangle.getGeometricCenter());
	startRectangle.setPosition({ windowWidth / 2, 200 });
	startRectangle.setFillColor(sf::Color::Black);
	startRectangle.setOutlineThickness(5);
	startRectangle.setOutlineColor(sf::Color::White);

	sf::RectangleShape exitRectangle({ buttonWidth, buttonHeight });
	exitRectangle.setOrigin(exitRectangle.getGeometricCenter());
	exitRectangle.setPosition({ windowWidth / 2, startRectangle.getPosition().y + (float)buttonHeight / 2 + 100.f });
	exitRectangle.setFillColor(sf::Color::Black);
	exitRectangle.setOutlineThickness(5);
	exitRectangle.setOutlineColor(sf::Color::White);

	sf::Font font;
	if (!font.openFromFile("Assets/Fonts/game_font.ttf"))
	{
		std::cout << "Error: Could not find font file in assets folder!" << std::endl;
	}
	sf::Text startText(font);
	startText.setString("START GAME");
	startText.setCharacterSize(40);
	startText.setFillColor(sf::Color::White);
	sf::FloatRect startTextBounds = startText.getLocalBounds();
	startText.setOrigin({ startTextBounds.size.x / 2, startTextBounds.size.y });
	startText.setPosition(startRectangle.getPosition());

	sf::Text exitText(font);
	exitText.setString("EXIT");
	exitText.setCharacterSize(40);
	exitText.setFillColor(sf::Color::White);
	sf::FloatRect exitTextBounds = exitText.getLocalBounds();
	exitText.setOrigin({ exitTextBounds.size.x / 2, exitTextBounds.size.y });
	exitText.setPosition(exitRectangle.getPosition());

	// Game
	sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "PONG GAME");
	window.setFramerateLimit(60);

	sf::CircleShape ball(5.f);
	ball.setOrigin(ball.getGeometricCenter());
	ball.setPosition({ windowWidth / 2, windowHeight - (ballRadius + playerHeight) });
	ball.setFillColor(sf::Color::White);

	sf::RectangleShape player({ playerLength, playerHeight });
	player.setOrigin(player.getGeometricCenter());
	player.setPosition({ windowWidth / 2, windowHeight - playerHeight / 2 });
	player.setFillColor(sf::Color::Green);

	sf::RectangleShape enemy({ playerLength, playerHeight });
	enemy.setOrigin(enemy.getGeometricCenter());
	enemy.setPosition({ windowWidth / 2, 0 + playerHeight / 2 });
	enemy.setFillColor(sf::Color::Red);

	sf::Clock clock;
	int playerHorizontalDirection;
	int enemyHorizontalDirection = 1;

	sf::Angle startingAngle = generateRandomStartingAngle(initialMinAngle, initialMaxAngle);

	float ballHorizotalDirection = std::cos(startingAngle.asRadians());
	float ballVerticalDirection = std::sin(startingAngle.asRadians());
	float ballCurrentVelocity = ballInitialVelocity;

	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			} 
			else if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Escape)
				{
					window.close();
				}
			}

			if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseClick->button == sf::Mouse::Button::Left)
				{
					if (gameState != GameState::Menu) break;

					sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
					sf::Vector2f mouseWorldPos = window.mapPixelToCoords(pixelPosition);

					if (startRectangle.getGlobalBounds().contains(mouseWorldPos))
					{
						gameState = GameState::Playing;

						selectSound.play();
						backgroundMusic.setVolume(50.f);
						backgroundMusic.setLooping(true);
						backgroundMusic.play();
					}
					if (exitRectangle.getGlobalBounds().contains(mouseWorldPos))
					{
						selectSound.play();
						window.close();
					}
				}
			}
		}

		window.clear();

		if (gameState == GameState::Menu)
		{
			sf::Vector2i pixelPosition = sf::Mouse::getPosition(window);
			sf::Vector2f mouseWorldPos = window.mapPixelToCoords(pixelPosition);

			// This is hovering
			if (startRectangle.getGlobalBounds().contains(mouseWorldPos))
			{
				startRectangle.setFillColor(sf::Color::White);
				startText.setFillColor(sf::Color::Black);
				startRectangle.setScale({ 1.05f, 1.05f });
			}
			else
			{
				startRectangle.setFillColor(sf::Color::Black);
				startText.setFillColor(sf::Color::White);
				startRectangle.setScale({ 1.0f, 1.0f });
			}

			if (exitRectangle.getGlobalBounds().contains(mouseWorldPos))
			{
				exitRectangle.setFillColor(sf::Color::White);
				exitText.setFillColor(sf::Color::Black);
				exitRectangle.setScale({ 1.05f, 1.05f });
			}
			else
			{
				exitRectangle.setFillColor(sf::Color::Black);
				exitText.setFillColor(sf::Color::White);
				exitRectangle.setScale({ 1.0f, 1.0f });
			}

			window.draw(startRectangle);
			window.draw(startText);

			window.draw(exitRectangle);
			window.draw(exitText);
		}
		else if (gameState == GameState::Playing)
		{
			playerHorizontalDirection = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
			{
				playerHorizontalDirection = -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
			{
				playerHorizontalDirection = 1;
			}

			// Player movement
			float playerNewPosition = player.getPosition().x + playerVelocity * deltaTime * playerHorizontalDirection;
			playerNewPosition = std::clamp(playerNewPosition, 0 + playerLength / 2, windowWidth - playerLength / 2);
			player.setPosition({ playerNewPosition, player.getPosition().y });

			// Enemy movement
			float currentEnemyStep = playerVelocity * deltaTime;
			float distance = ball.getPosition().x - enemy.getPosition().x;
			float enemyNewPosition;
			if (abs(distance) > currentEnemyStep)
			{
				enemyHorizontalDirection = distance > 1 ? 1 : -1;
				enemyNewPosition = enemy.getPosition().x + currentEnemyStep * enemyHorizontalDirection;
			}
			else
			{
				enemyNewPosition = ball.getPosition().x;
			}

			enemyNewPosition = (std::clamp(enemyNewPosition, 0 + playerLength / 2, windowWidth - playerLength / 2));
			enemy.setPosition({ enemyNewPosition, enemy.getPosition().y });

			// Ball movement
			// 1. First check if ball is touching the walls and set direction
			if (ball.getPosition().x + ballRadius >= windowWidth
				|| ball.getPosition().x <= 0 + ballRadius)
			{
				ballHorizotalDirection *= -1;
			}

			// 2. Check if the ball is touching a player/enemy
			float newPitch = 1.0f;
			if (ball.getGlobalBounds().findIntersection(player.getGlobalBounds()))
			{
				ball.setPosition({ ball.getPosition().x, player.getPosition().y - (ballRadius + playerHeight / 2) });
				ballVerticalDirection *= -1;
				ballCurrentVelocity += ballCurrentVelocity * 0.01;

				// Increasing music pitch
				paddleHitSound.play();
				newPitch *= 1.01f;
				backgroundMusic.setPitch(newPitch);
			}
			else if (ball.getGlobalBounds().findIntersection(enemy.getGlobalBounds()))
			{
				ball.setPosition({ ball.getPosition().x, enemy.getPosition().y + (ballRadius + playerHeight / 2) });
				ballVerticalDirection *= -1;
				ballCurrentVelocity += ballCurrentVelocity * 0.01;

				// Increasing music pitch
				paddleHitSound.play();
				newPitch *= 1.01f;
				backgroundMusic.setPitch(newPitch);
			}

			// 3. Check if the ball is touching the dead zone
			if (ball.getPosition().y - ballRadius <= 0
				|| ball.getPosition().y + ballRadius >= windowHeight)
			{
				// Reset players:
				player.setPosition({ windowWidth / 2, windowHeight - playerHeight / 2 });
				enemy.setPosition({ windowWidth / 2, 0 + playerHeight / 2 });
				enemyHorizontalDirection = 1;

				// Reset ball:
				ballCurrentVelocity = ballInitialVelocity;
				ball.setPosition({ windowWidth / 2, player.getPosition().y - (ballRadius + playerHeight / 2) });

				sf::Angle startingAngle = generateRandomStartingAngle(initialMinAngle, initialMaxAngle);
				ballHorizotalDirection = std::cos(startingAngle.asRadians());
				ballVerticalDirection = std::sin(startingAngle.asRadians());

				backgroundMusic.stop();

				gameState = GameState::Menu;

				continue;
			}

			// 4. Calculate the new position
			float ballX = ball.getPosition().x;
			float ballY = ball.getPosition().y;
			float ballNewX = ballX + ballCurrentVelocity * deltaTime * ballHorizotalDirection;
			float ballNewY = ballY + ballCurrentVelocity * deltaTime * ballVerticalDirection;

			// 5. Set the new position
			ball.setPosition({ ballNewX, ballNewY });

			window.draw(player);
			window.draw(enemy);
			window.draw(ball);
		}

		window.display();
	}
}