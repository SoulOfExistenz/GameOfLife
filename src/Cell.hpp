#pragma once
#include <cassert>

class Cell
{
private:
	bool m_alive{ false };
	int m_neighbours{ 0 };

	void addNeighbours() { m_neighbours++; }
	void decreaseNeighbours() { assert(m_neighbours > 0); { m_neighbours--; } }

public:
	Cell() = default;

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
};