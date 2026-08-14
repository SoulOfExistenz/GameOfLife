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

	size_t gridToIndex(sf::Vector2i pos)
	{
		return pos.x + pos.y + (pos.y * (Settings::cellAmount - 1));
	}

public:
	Board(sf::RenderWindow& window) : m_window(window)
	{
		auto cellSize = Settings::cellSize;
		auto cellAmount = Settings::cellAmount;
		//create cells
		for (float column{ 0 }; column < cellAmount; column++)
		{
			float yPos{ cellSize / 2 + (column * cellSize) };

			for (float row{ 0 }; row < cellAmount; row++)
			{
				float xPos{ cellSize / 2 + (row * cellSize) };
				m_board.emplace_back(sf::Vector2f{ xPos, yPos });
			}
		}
	}

	void draw()
	{
		for (auto& e : m_board)
			e.draw(m_window);
	}

	void setCell(const sf::Vector2i gridPos, bool isLeftClick)
	{
		auto index = gridToIndex(gridPos);
		std::cout << "Index: " << index << '\n';

		//effect cell
		auto& cell = m_board[index];
		cell.setAlive(isLeftClick);

		//effect neighbours
		size_t leftIndex{ index - 1 };
		size_t rightIndex{ index + 1 };
		size_t upIndex{ index - Settings::cellAmount };
		size_t downIndex{ index + Settings::cellAmount };

		//left wrap
		if (gridPos.x == 0)
		{
			leftIndex = index + Settings::cellAmount - 1;
		}
		//Right wrap
		if (gridPos.x == Settings::cellAmount - 1)
			rightIndex = index - Settings::cellAmount + 1;
		//up wrap
		if (gridPos.y == 0)
			upIndex = index + (Settings::cellAmount - 1) * (Settings::cellAmount - 1) + Settings::cellAmount - 1;
		//down wrap
		if (gridPos.y == Settings::cellAmount - 1)
			downIndex = index - (Settings::cellAmount - 1 ) * (Settings::cellAmount);

		m_board[leftIndex].addNeighbours();
		m_board[rightIndex].addNeighbours();
		m_board[upIndex].addNeighbours();
		m_board[downIndex].addNeighbours();
		//upRight
		m_board[upIndex - (index - rightIndex)].addNeighbours();
		//upLeft
		m_board[upIndex - (index - leftIndex)].addNeighbours();
		//downRight
		m_board[downIndex - (index - rightIndex)].addNeighbours();
		//downLeft
		m_board[downIndex - (index - leftIndex)].addNeighbours();




		//Helper
		std::cout << "leftIndex: " << leftIndex << '\n';
		std::cout << "rightIndex: " << rightIndex << '\n';
		std::cout << "upIndex: " << upIndex << '\n';
		std::cout << "downIndex: " << downIndex << '\n';
		std::cout << "upRight: " << upIndex - (index - rightIndex) << '\n';
		std::cout << "upLeft: " << upIndex - (index - leftIndex) << '\n';
		std::cout << "bottomRight: " << downIndex - (index - rightIndex) << '\n';
		std::cout << "bottomLeft: " << downIndex - (index - leftIndex) << '\n';
		
		std::cout << "Neighbours: " << cell.getNeighbours() << '\n';
	}
};