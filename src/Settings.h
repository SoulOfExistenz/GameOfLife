#pragma once
#include <SFML/graphics.hpp>

namespace Settings
{
	constexpr int cellAmount{ 100 };
	constexpr int cellSize{ 5 };
	constexpr int WINDOW_SIZE{ cellAmount * cellSize };

	inline float tick{ 0.1f };
	inline float randomizeAmount{ 0.25f };

	constexpr sf::Color aliveColor{ sf::Color::Red};
	constexpr sf::Color deadColor{ sf::Color::Black };
	constexpr sf::Color outlineColor{ sf::Color::White };
}
