#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.h"

class Cell
{
private:
	sf::RectangleShape m_shape{ {Settings::cellSize, Settings::cellSize} };

	bool m_alive{ false };
	int m_neighbours{ 0 };

public:
	Cell(sf::Vector2f pos)
	{
		m_shape.setFillColor(m_alive ? Settings::aliveColor : Settings::deadColor);
		m_shape.setPosition(pos);
		m_shape.setOrigin(m_shape.getGeometricCenter());
		m_shape.setOutlineColor(Settings::outlineColor);
		m_shape.setOutlineThickness(2);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_shape);
	}

	void setAlive(bool b)
	{
		m_alive = b;
		m_shape.setFillColor(m_alive ? Settings::aliveColor : Settings::deadColor);
	}

	int getNeighbours() const { return m_neighbours; }
	void addNeighbours() { m_neighbours++; }

};