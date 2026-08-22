#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Settings.h"
#include "Board.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ Settings::WINDOW_SIZE, Settings::WINDOW_SIZE }), "Game Of Life");

	Board board{ window };

	sf::Clock clock;
	bool isLeftMouseHeld{ false };
	bool isRightMouseHeld{ false };
	sf::Vector2i mPos;
	

	if (ImGui::SFML::Init(window))
		std::cout << "ImGui init success\n";
	else
		std::cout << "ImGui Init failed\n";

	while (window.isOpen())
	{

		//IMGUI
		ImGui::SFML::Update(window, clock.restart());
		ImGui::Begin("Game of Life");
		ImGui::Text("Generation Count: %d", board.getGeneration());
		ImGui::Text("Cell Count: %d", Settings::cellAmount * Settings::cellAmount);
		ImGui::Text("ScreenSize: %d", window.getSize().x);
		if (ImGui::Button("Start/Pause"))
		{
			board.togglePause();
		}
		ImGui::SameLine();
		ImGui::SliderFloat("Ticks", &Settings::tick, 0.f, 1.f);
		if (ImGui::Button("Randomize"))
			board.randomizeGrid();
		ImGui::SameLine();
		ImGui::SliderFloat("Amount", &Settings::randomizeAmount, 0.f, 1.f);
		ImGui::End();

		//INPUT
		while (const auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>())
				window.close();

			//Mouse Moved
			if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
			{
				//mPos = mouseMoved->position / Settings::cellSize;
				auto mappedMouse = window.mapPixelToCoords(mouseMoved->position);
				mPos = static_cast<sf::Vector2i>(mappedMouse) / Settings::cellSize;
			}
			
			//Mouse Pressed
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (ImGui::GetIO().WantCaptureMouse)
					continue;
				
				if(mouseButtonPressed->button == sf::Mouse::Button::Left)
					isLeftMouseHeld = true;

				if (mouseButtonPressed->button == sf::Mouse::Button::Right)
					isRightMouseHeld = true;

				auto pos = mouseButtonPressed->position;
				std::cout << '\n';
				std::cout << "Raw Mouse Position: " << pos.x << ", " << pos.y << '\n';
				std::cout << "Grid: " << pos.x / Settings::cellSize << ", " << pos.y / Settings::cellSize << '\n';
			}

			//Mouse Released
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
			{
				if(mouseButtonReleased->button == sf::Mouse::Button::Left)
					isLeftMouseHeld = false;
				
				if (mouseButtonReleased->button == sf::Mouse::Button::Right)
					isRightMouseHeld = false;
			}

			if (event->is<sf::Event::MouseLeft>())
			{
				isLeftMouseHeld = false;
				isRightMouseHeld = false;
			}
			//Key Pressed
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scan::Space)
				{
					board.togglePause();
				}

				if (keyPressed->scancode == sf::Keyboard::Scan::R) 
					board.randomizeGrid();

				if (keyPressed->scancode == sf::Keyboard::Scan::C)
					board.clear();

				if (keyPressed->scancode == sf::Keyboard::Scan::G)
					board.toggleGrid();
			}
		}
		
		//UPDATE
		if (isLeftMouseHeld)
		{
			board.setCell(mPos, true, true);
		}
		else if (isRightMouseHeld)
		{
			board.setCell(mPos, false, true);
		} 


		board.update();

		//RENDERING
		window.clear();

		board.drawCellsVertices();

		board.drawGrid();

		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();
}