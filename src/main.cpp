#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Settings.h"
#include "Board.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ Settings::WINDOW_SIZE, Settings::WINDOW_SIZE }), "Game Of Life");
	sf::Clock clock;

	Board board{ window };

	if (ImGui::SFML::Init(window))
		std::cout << "ImGui init success\n";
	else
		std::cout << "ImGui Init failed\n";

	while (window.isOpen())
	{
		
		//input
		while (const auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>())
				window.close();

			//Mouse Pressed
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				auto mPos = mouseButtonPressed->position;
				std::cout << '\n';
				std::cout << "Raw Mouse Position: " << mPos.x << ", " << mPos.y << '\n';
				std::cout << "Grid: " << mPos.x / Settings::cellSize << ", " << mPos.y / Settings::cellSize << '\n';

				board.setCell(mPos / Settings::cellSize, mouseButtonPressed->button == sf::Mouse::Button::Left);
			}
		}

		ImGui::SFML::Update(window, clock.restart());

		//rendering
		window.clear();

		board.draw();

		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();
}


//RULES//

//SURVIVAL: Any live cell with 2 or 3 live neighbours survives.
//REPRODUCTION:  Any dead cell with exactly 3 live neighbors becomes a live cell.
//OVERPOPULATION: Any live cell with more than 3 live neighbours dies.
//UNDERPOPULATION: Any live cell with fewer than 2 live neighbours dies.