#pragma once
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <iostream>

class AudioManager
{
	const float DAMPING_FACTOR = 0.3f;
	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::optional<sf::Sound> sound;
	sf::Music backgroundMusic;

public:
	AudioManager();

	void PlaySound(const std::string_view& soundName);

	void PlayBackgroundMusic();

	void SetPitch(float speedRatio);

	void StopBackgroundMusic();
};