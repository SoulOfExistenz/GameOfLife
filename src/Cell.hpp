#pragma once
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <cassert>

class Cell
{
private:
	sf::Vector2f m_pos;
	bool m_alive{ false };
	int m_neighbours{ 0 };

	void addNeighbours() { m_neighbours++; }
	void decreaseNeighbours() { assert(m_neighbours > 0); { m_neighbours--; } }

public:
	Cell(sf::Vector2f pos) : m_pos{pos}
	{
	}

	void setState(bool b)
	{
		m_alive = b;
	}

	void updateNeighbourCount(bool increment)
	{
		if (increment)
			addNeighbours();
		else
			decreaseNeighbours();
	}

	bool getState() const { return m_alive; }
	int getNeighbours() const { return m_neighbours; }
	const sf::Vector2i getGridPos() const { return sf::Vector2i{ m_pos } / Settings::cellSize; }
	const sf::Vector2f getPos() const { return m_pos; }
};