#pragma once
#include "Cell.hpp"
#include "Random.h"
#include "Settings.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>

Board::Board(sf::RenderWindow& window) : m_window(window), m_boardA(Settings::cellAmount * Settings::cellAmount), m_boardB(Settings::cellAmount * Settings::cellAmount)
									   , m_gridVertices(sf::PrimitiveType::Lines), m_cellVertices(sf::PrimitiveType::Triangles)
{
	float cellSize = Settings::cellSize;
	float cellAmount = Settings::cellAmount;

	//init grid
	for (auto i{ 0 }; i < (cellAmount - 1); i++)
	{
		auto color = Settings::gridColor;
		//horizontal lines
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ 0.0f,				  (i + 1) * cellSize }, color));
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ cellAmount * cellSize, (i + 1) * cellSize }, color));
		//vertical lines
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ (i + 1) * cellSize, 0.0f }, color));
		m_gridVertices.append(sf::Vertex(sf::Vector2f{ (i + 1) * cellSize, cellAmount * cellSize }, color));
	}

	//Init Vertices
	for (auto i{ 0 }; i < std::size(m_boardA); i++)
	{
		sf::Vector2f pos = {static_cast<float>(i % Settings::cellAmount * Settings::cellSize), static_cast<float>(i / Settings::cellAmount * Settings::cellSize)};
		auto color = Settings::deadColor;

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
	for (auto i{0}; i < std::size(m_boardA); i++)
	{
		if (m_currentBoard->at(i).getState())
		{
			setCell(indexToGridPos(i), false, true);
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
			const auto randomIndex{ Random::get<int>(0, std::size(m_boardA) - 1 )};
			const auto& cell = m_currentBoard->at(randomIndex);

			if (cell.getState())
				continue;

			setCell(indexToGridPos(randomIndex), true, true);
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
		auto gridPos = indexToGridPos(i);

		//if cell is dead and has 3 live neighbours it becomes alive
		if (!curCell.getState())
		{
			if (m_nextBoard->at(i).getState())
				setCell(gridPos, false);

			if (curCell.getNeighbours() == 3)
			{
				setCell(gridPos, true);
				continue;
			}
		}

		//If cell is alive
		if (curCell.getState())
		{
			if (!m_nextBoard->at(i).getState())
				setCell(gridPos, true);

			//If cell has less than 2 or more than 3 neighbours it dies
			if (curCell.getNeighbours() < 2 || curCell.getNeighbours() > 3)
			{
				setCell(gridPos, false);
				continue;
			}
		}
	}

	//swap boards
	std::swap(m_currentBoard, m_nextBoard);
}

void Board::setCell(const sf::Vector2i gridPos, bool state, bool currentBoard)
{
	if (gridPos.x < 0 || gridPos.y < 0)
		return;
	if (gridPos.x > Settings::cellAmount - 1 || gridPos.y > Settings::cellAmount - 1)
		return;

	//get index
	auto index = gridToIndex(gridPos);
	auto cellAmount = Settings::cellAmount;

	if (index > std::size(m_boardA) - 1)
	{
		std::cout << "setCell() index doesn't fit\n";
		return;
	}

	auto& board = (currentBoard ? m_currentBoard : m_nextBoard);
	auto& cell = board->at(index);

	if (cell.getState() == state) { return; }

	//effect cell
	cell.setState(state);

	//color vertices
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

	//effect neighbours neighbour count
	board->at(leftIndex).updateNeighbourCount(state);
	board->at(rightIndex).updateNeighbourCount(state);
	board->at(upIndex).updateNeighbourCount(state);
	board->at(downIndex).updateNeighbourCount(state);
	//upRight
	board->at(upIndex - (index - rightIndex)).updateNeighbourCount(state);
	//upLeft
	board->at(upIndex - (index - leftIndex)).updateNeighbourCount(state);
	//downRight
	board->at(downIndex - (index - rightIndex)).updateNeighbourCount(state);
	//downLeft
	board->at(downIndex - (index - leftIndex)).updateNeighbourCount(state);

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
