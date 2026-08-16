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
	float time{};
	bool isPlay{ false };
	Board board{ window };

	if (ImGui::SFML::Init(window))
		std::cout << "ImGui init success\n";
	else
		std::cout << "ImGui Init failed\n";

	while (window.isOpen())
	{
		time += clock.getElapsedTime().asSeconds();

		if (time > Settings::tick && isPlay)
		{
			board.nextGen();
			time -= Settings::tick;
		}

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

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Space)
				{
					isPlay = !isPlay;
					time = 0;
					std::cout << (isPlay ? "Playing" : "Paused");
				}
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