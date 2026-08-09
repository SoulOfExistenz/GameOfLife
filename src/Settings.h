#pragma once
#include <SFML/graphics.hpp>

namespace Settings
{
	constexpr int WINDOW_SIZE{ 500 };
	constexpr int cellAmount{ 20 };
	constexpr int cellSize{ 500 / cellAmount };

	constexpr sf::Color aliveColor{ sf::Color::Red };
	constexpr sf::Color deadColor{ sf::Color::Black };
	constexpr sf::Color outlineColor{ sf::Color::White };
}
