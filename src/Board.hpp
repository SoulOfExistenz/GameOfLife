#pragma once
#include "Cell.hpp"
#include "Random.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Board
{
private:
	std::vector<Cell> m_board;
	std::vector<Cell> m_nextBoard;

	std::vector<Cell>* m_currentBoard{ &m_board };

	int m_generationCount{ 0 };
	sf::RenderWindow& m_window;

	size_t gridToIndex(sf::Vector2i pos)
	{
		return pos.x + (pos.y * (Settings::cellAmount));
	}

public:
	Board(sf::RenderWindow& window) : m_window(window)
	{
		auto cellSize = Settings::cellSize;
		auto cellAmount = Settings::cellAmount;
		//Init board
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

	int getGeneration() const { return m_generationCount; }

	void clear()
	{
		for (auto e : m_board)
		{
			if(e.getState())
			setCell(e.getGridPos(), false);
		}
		m_generationCount = 0;
	}

	void randomizeGrid()
	{
		clear();
		for (auto i{ 0 }; i < std::size(m_board) * Settings::randomizeAmount; i++)
		{
			do
			{
				auto cell = m_board[Random::get<size_t>(0, std::size(m_board) - 1)];

				if (cell.getState())
					continue;

				setCell(cell.getGridPos(), true);
				break;

			} while (true);
		}
	}

	void draw()
	{
		for (auto& e : m_board)
			e.draw(m_window);
	}

	void nextGen()
	{
		m_nextBoard = m_board;
		m_generationCount++;
		//loop through the tempBoard and make changes to board
		for (auto i{0}; i < std::size(m_nextBoard); i++)
		{
			auto tempCell = m_nextBoard[i];

			//if cell is dead and has 3 live neighbours it becomes alive
			if (!tempCell.getState() && tempCell.getNeighbours() == 3)
			{
				setCell(tempCell.getGridPos(), true);
				continue;
			}

			//If cell is alive
			if (tempCell.getState())
			{
				//If cell has less than 2 or more than 3 neighbours it dies
				if (tempCell.getNeighbours() < 2 || tempCell.getNeighbours() > 3)
				{
					setCell(tempCell.getGridPos(), false);
					continue;
				}
			}
		}
	}

	void setCell(const sf::Vector2i gridPos, bool state)
	{
		//get index
		auto index = gridToIndex(gridPos);

		if (index > m_board.size() - 1)
		{
			std::cout << "Too big\n";
			return;
		}

		auto& cell = m_board[index];


		if (cell.getState() == state)
		{
			//std::cout << "This is already set as you want it to be...\n";
			return;
		}

		//std::cout << "Neighbours: " << cell.getNeighbours() << '\n';
		
		//effect cell
		cell.setState(state);

		//effect neighbours
		size_t leftIndex{ index - 1 };
		size_t rightIndex{ index + 1 };
		size_t upIndex{ index - Settings::cellAmount };
		size_t downIndex{ index + Settings::cellAmount };

		//left wrap
		if (gridPos.x == 0)
			leftIndex = index + Settings::cellAmount - 1;
		//Right wrap
		if (gridPos.x == Settings::cellAmount - 1)
			rightIndex = index - Settings::cellAmount + 1;
		//up wrap
		if (gridPos.y == 0)
			upIndex = index + (Settings::cellAmount - 1) * (Settings::cellAmount);
		//down wrap
		if (gridPos.y == Settings::cellAmount - 1)
			downIndex = index - (Settings::cellAmount - 1 ) * (Settings::cellAmount);

		if (state)
		{
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
		}
		else
		{
			m_board[leftIndex].decreaseNeighbours();
			m_board[rightIndex].decreaseNeighbours();
			m_board[upIndex].decreaseNeighbours();
			m_board[downIndex].decreaseNeighbours();
			//upRight
			m_board[upIndex - (index - rightIndex)].decreaseNeighbours();
			//upLeft
			m_board[upIndex - (index - leftIndex)].decreaseNeighbours();
			//downRight
			m_board[downIndex - (index - rightIndex)].decreaseNeighbours();
			//downLeft
			m_board[downIndex - (index - leftIndex)].decreaseNeighbours();
		}

		//Helper
		//std::cout << "leftIndex: " << leftIndex << '\n';
		//std::cout << "rightIndex: " << rightIndex << '\n';
		//std::cout << "upIndex: " << upIndex << '\n';
		//std::cout << "downIndex: " << downIndex << '\n';
		//std::cout << "upRight: " << upIndex - (index - rightIndex) << '\n';
		//std::cout << "upLeft: " << upIndex - (index - leftIndex) << '\n';
		//std::cout << "bottomRight: " << downIndex - (index - rightIndex) << '\n';
		//std::cout << "bottomLeft: " << downIndex - (index - leftIndex) << '\n';
	}
};