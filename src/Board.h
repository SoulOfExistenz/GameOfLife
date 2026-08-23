#pragma once
#include "Cell.hpp"
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Board
{
private:
	std::vector<Cell> m_boardA;
	std::vector<Cell> m_boardB;

	std::vector<Cell>* m_currentBoard{ &m_boardA };
	std::vector<Cell>* m_nextBoard{ &m_boardB };

	sf::VertexArray m_gridVertices;
	sf::VertexArray m_cellVertices;

	sf::Clock m_clock;
	float m_time{};
	bool m_isPlay{ false };
	bool m_isDrawGrid{ false };

	int m_generationCount{ 0 };
	sf::RenderWindow& m_window;

	size_t gridToIndex(sf::Vector2i pos) { return (pos.y * Settings::cellAmount) + pos.x; }

	void nextGen();

public:
	Board(sf::RenderWindow& window);

	void update();

	void clear();

	void randomizeGrid();

	void setCell(const sf::Vector2i gridPos, bool state, bool currentBoard = false);

	int getGeneration() const { return m_generationCount; }

	void toggleGrid() { m_isDrawGrid = !m_isDrawGrid; }

	void drawGrid()
	{
		if(m_isDrawGrid)
		m_window.draw(m_gridVertices);
	}

	void drawCellsVertices()
	{
		m_window.draw(m_cellVertices);
	}

	void togglePause()
	{
		m_isPlay = !m_isPlay;
		m_time = 0;
		std::cout << (m_isPlay ? "Playing" : "Paused") << '\n';
	}

};