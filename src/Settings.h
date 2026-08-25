#pragma once
#include <SFML/graphics.hpp>

namespace Settings
{
	constexpr int cellAmount{ 1000 };
	constexpr int cellSize{ 20 };
	constexpr int WINDOW_SIZE{ 800 };
	constexpr int gridSize{ cellAmount * cellSize };

	inline float tick{ 0.1f };
	inline float randomizeAmount{ 0.25f };

	constexpr sf::Color aliveColor{ sf::Color::Red };
	constexpr sf::Color deadColor{ sf::Color::Black };
	constexpr sf::Color gridColor{ sf::Color::White };
}
