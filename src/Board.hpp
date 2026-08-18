#pragma once
#include "Cell.hpp"
#include "Random.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <utility>

class Board
{
private:
	std::vector<Cell> m_boardA;
	std::vector<Cell> m_boardB;

	std::vector<Cell>* m_currentBoard{ &m_boardA };
	std::vector<Cell>* m_nextBoard{ &m_boardB };

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
				m_boardA.emplace_back(sf::Vector2f{ xPos, yPos });
				m_boardB.emplace_back(sf::Vector2f{ xPos, yPos });
			}
		}
	}

	int getGeneration() const { return m_generationCount; }

	void clear()
	{
		for (auto& e : *m_currentBoard)
		{
			if (e.getState())
			{
				setCell(e.getGridPos(), false, true);
			}
		}
		m_generationCount = 0;
	}

	void randomizeGrid()
	{
		clear();
		for (auto i{ 0 }; i < std::size(m_boardA) * Settings::randomizeAmount; i++)
		{
			do
			{
				const auto& cell = m_currentBoard->at(Random::get<size_t>(0, std::size(m_boardA) - 1));

				if (cell.getState())
					continue;

				setCell(cell.getGridPos(), true, true);
				break;

			} while (true);
		}
	}

	void draw()
	{
		for (auto& e : *m_currentBoard)
			e.draw(m_window);
	}


	void nextGen()
	{
		m_generationCount++;

		// make changes to the next board based on the state of the current board
		for (auto i{ 0 }; i < std::size(*m_currentBoard); i++)
		{
			auto& curCell = m_currentBoard->at(i);

			//if cell is dead and has 3 live neighbours it becomes alive
			if (!curCell.getState())
			{
				if(m_nextBoard->at(i).getState())
				setCell(curCell.getGridPos(), false);

				if (curCell.getNeighbours() == 3)
				{
					setCell(curCell.getGridPos(), true);
					continue;
				}
			}

			//If cell is alive
			if (curCell.getState())
			{
				if(!m_nextBoard->at(i).getState())
				setCell(curCell.getGridPos(), true);

				//If cell has less than 2 or more than 3 neighbours it dies
				if (curCell.getNeighbours() < 2 || curCell.getNeighbours() > 3)
				{
					setCell(curCell.getGridPos(), false);
					continue;
				}
			}
		}

		//swap boards
		std::swap(m_currentBoard, m_nextBoard);
	}

	void setCell(const sf::Vector2i gridPos, bool state, bool currentBoard = false)
	{
		//get index
		auto index = gridToIndex(gridPos);

		if (index > std::size(m_boardA) - 1)
		{
			std::cout << "Too big\n";
			return;
		}

		auto board = (currentBoard ? m_currentBoard : m_nextBoard);
		auto& cell = board->at(index);

		if (cell.getState() == state)
		{
			//std::cout << "This is already set as you want it to be...\n";
			return;
		}

		//std::cout << "Neighbours: " << cell.getNeighbours() << '\n';
		
		//effect cell
		cell.setState(state);

		//get neighbour indices
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

		//effect neighbours
		if (state)
		{
			board->at(leftIndex).addNeighbours();
			board->at(rightIndex).addNeighbours();
			board->at(upIndex).addNeighbours();
			board->at(downIndex).addNeighbours();
			//upRight
			board->at(upIndex - (index - rightIndex)).addNeighbours();
			//upLeft
			board->at(upIndex - (index - leftIndex)).addNeighbours();
			//downRight
			board->at(downIndex - (index - rightIndex)).addNeighbours();
			//downLeft
			board->at(downIndex - (index - leftIndex)).addNeighbours();
		}
		else
		{
			board->at(leftIndex).decreaseNeighbours();
			board->at(rightIndex).decreaseNeighbours();
			board->at(upIndex).decreaseNeighbours();
			board->at(downIndex).decreaseNeighbours();
			//upRight
			board->at(upIndex - (index - rightIndex)).decreaseNeighbours();
			//upLeft
			board->at(upIndex - (index - leftIndex)).decreaseNeighbours();
			//downRight
			board->at(downIndex - (index - rightIndex)).decreaseNeighbours();
			//downLeft
			board->at(downIndex - (index - leftIndex)).decreaseNeighbours();
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