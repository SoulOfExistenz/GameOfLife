#pragma once
#include <SFML/graphics.hpp>

namespace Settings
{
	constexpr int WINDOW_SIZE{ 800 };
	constexpr int cellAmount{ 50 };
	constexpr int cellSize{ WINDOW_SIZE / cellAmount };
	constexpr float tick{ 0.5f };

	constexpr sf::Color aliveColor{ sf::Color::Red };
	constexpr sf::Color deadColor{ sf::Color::Black };
	constexpr sf::Color outlineColor{ sf::Color::White };
}
