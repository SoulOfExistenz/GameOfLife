#pragma once
#include "Cell.hpp"
#include "Random.h"
#include "Settings.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>

Board::Board(sf::RenderWindow& window) : m_window(window), m_gridVertices(sf::PrimitiveType::Lines), m_cellVertices(sf::PrimitiveType::Triangles)
{
	float cellSize = Settings::cellSize;
	float cellAmount = Settings::cellAmount;

	//init grid
	for (auto i{ 0 }; i < (cellAmount - 1); i++)
	{
		//horizontal lines
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ 0.0f,				  (i + 1) * cellSize }));
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ cellAmount * cellSize, (i + 1) * cellSize }));
		//vertical lines
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ (i + 1) * cellSize, 0.0f }));
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ (i + 1) * cellSize, cellAmount * cellSize }));
	}

	//Init board
	for (float column{ 0 }; column < cellAmount; column++)
	{
		float yPos{ column * cellSize };

		for (float row{ 0 }; row < cellAmount; row++)
		{
			float xPos{ row * cellSize };
			m_boardA.emplace_back(sf::Vector2f{ xPos, yPos });
			m_boardB.emplace_back(sf::Vector2f{ xPos, yPos });
		}
	}

	//Init Cell Vertices
	for (auto i{ 0 }; i < std::size(m_boardA); i++)
	{
		sf::Vector2f pos = static_cast<sf::Vector2f>(m_boardA[i].getPos());
		auto color = sf::Color::Black;

		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x, pos.y }, color)); // top left
		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x + cellSize, pos.y }, color)); // top right
		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x, pos.y + cellSize }, color)); // bottom left

		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x + cellSize, pos.y + cellSize }, color)); // bottom right
		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x, pos.y + cellSize }, color)); // bottom left
		m_cellVertices.append(sf::Vertex(sf::Vector2f{ pos.x + cellSize, pos.y }, color)); // top right
	}
}

void Board::clear()
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

void Board::randomizeGrid()
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

void Board::update()
{
	m_time += m_clock.restart().asSeconds();

	if (m_time >= Settings::tick && m_isPlay)
	{
		nextGen();
		m_time -= Settings::tick;
	}
}

void Board::nextGen()
{
	m_generationCount++;

	// make changes to the next board based on the state of the current board
	for (auto i{ 0 }; i < std::size(*m_currentBoard); i++)
	{
		auto& curCell = m_currentBoard->at(i);

		//if cell is dead and has 3 live neighbours it becomes alive
		if (!curCell.getState())
		{
			if (m_nextBoard->at(i).getState())
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
			if (!m_nextBoard->at(i).getState())
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

void Board::setCell(const sf::Vector2i gridPos, bool state, bool currentBoard)
{
	//get index
	auto index = gridToIndex(gridPos);
	auto cellAmount = Settings::cellAmount;

	if (index > std::size(m_boardA) - 1 || index < 0)
	{
		std::cout << "setCell() index doesn't fit\n";
		return;
	}

	auto& board = (currentBoard ? m_currentBoard : m_nextBoard);
	auto& cell = board->at(index);

	if (cell.getState() == state)
	{
		//std::cout << "This is already set as you want it to be...\n";
		return;
	}

	//std::cout << "Neighbours: " << cell.getNeighbours() << '\n';
	//std::cout << "Index: " << index;
	//effect cell
	cell.setState(state);

	//effect vertices
	for (auto i{ 0 }; i < 6; i++)
	{
		m_cellVertices[index * 6 + i].color = state ? Settings::aliveColor : Settings::deadColor;
	}

	//get neighbour indices
	size_t leftIndex{ index - 1 };
	size_t rightIndex{ index + 1 };
	size_t upIndex{ index - cellAmount };
	size_t downIndex{ index + cellAmount };

	//left wrap
	if (gridPos.x == 0)
		leftIndex = index + cellAmount - 1;
	//Right wrap
	if (gridPos.x == cellAmount - 1)
		rightIndex = index - cellAmount + 1;
	//up wrap
	if (gridPos.y == 0)
		upIndex = index + (cellAmount - 1) * cellAmount;
	//down wrap
	if (gridPos.y == cellAmount - 1)
		downIndex = index - (cellAmount - 1) * cellAmount;

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
