#pragma once
#include "Cell.hpp"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Board
{
private:
	std::vector<Cell> m_board;
	sf::RenderWindow& m_window;

	size_t posToIndex(sf::Vector2i pos)
	{
		pos = pos;
		size_t index{ 0 };
		index += pos.x + pos.y + (pos.y * (Settings::cellAmount - 1));
		return index;
	}

public:
	Board(sf::RenderWindow& window) : m_window(window)
	{
		auto cellSize = Settings::cellSize;
		auto cellAmount = Settings::cellAmount;

		for (float i{ 0 }; i < cellAmount; i++)
		{
			float yPos{ cellSize / 2 + (i * cellSize) };

			for (float j{ 0 }; j < cellAmount; j++)
			{
				float xPos{ cellSize / 2 + (j * cellSize) };
				m_board.emplace_back(sf::Vector2f{ xPos, yPos });
			}
		}
	}

	void draw()
	{
		for (auto& e : m_board)
			e.draw(m_window);
	}

	void handleClick(sf::Vector2i v, bool b)
	{
		auto index = posToIndex(v);
		std::cout << index << '\n';
		m_board[index].setAlive(b);
	}
};