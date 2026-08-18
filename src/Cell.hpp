#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include <iostream>

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
		//m_shape.setOutlineColor(Settings::outlineColor);
		//m_shape.setOutlineThickness(0.7f);
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(m_shape);
	}

	void setState(bool b)
	{
		m_alive = b;
		m_shape.setFillColor(m_alive ? Settings::aliveColor : Settings::deadColor);
	}
	void addNeighbours() { m_neighbours++; }
	void decreaseNeighbours() { if (m_neighbours > 0) { m_neighbours--; } }

	bool getState() const { return m_alive; }
	int getNeighbours() const { return m_neighbours; }
	const sf::Vector2i getGridPos() const { return sf::Vector2i{ m_shape.getPosition() } / Settings::cellSize; }
};