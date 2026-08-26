#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Settings.h"
#include "Board.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ Settings::WINDOW_SIZE, Settings::WINDOW_SIZE }), "Game Of Life");
	sf::View view{ {Settings::gridSizeInPixels / 2, Settings::gridSizeInPixels / 2  }, {Settings::gridSizeInPixels, Settings::gridSizeInPixels} };

	Board board{ window };

	sf::Clock clock;

	bool isLeftMouseHeld{ false };
	bool isRightMouseHeld{ false };
	bool isUpHeld{ false };
	bool isDownHeld{ false };
	bool isRightHeld{ false };
	bool isLeftHeld{ false };
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

			//Mouse Scroll
			if (const auto* mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				auto scrollDelta = mouseScrolled->delta;
				if (scrollDelta > 0)
					view.zoom(0.9f);
				else if (scrollDelta < 0)
					view.zoom(1.1f);

				//zoom restraints
				if (view.getSize().x > Settings::gridSizeInPixels)
				{
					view.setSize({ Settings::gridSizeInPixels, Settings::gridSizeInPixels });
				}

				if (view.getSize().x < Settings::cellSize * 2)
				{
					view.setSize({ Settings::cellSize * 2, Settings::cellSize * 2 });
				}
				
				float rightSideView = view.getCenter().x + view.getSize().x / 2;
				float leftSideView = view.getCenter().x - view.getSize().x / 2;
				float bottomSideView = view.getCenter().y + view.getSize().y / 2;
				float topSideView = view.getCenter().y - view.getSize().y / 2;

				float newPosX = view.getCenter().x;
				float newPosY = view.getCenter().y;

				if (rightSideView > Settings::gridSizeInPixels)
					newPosX -= rightSideView - Settings::gridSizeInPixels;

				if (leftSideView < 0)
					newPosX -= leftSideView;

				if (bottomSideView > Settings::gridSizeInPixels)
					newPosY -= bottomSideView - Settings::gridSizeInPixels;

				if (topSideView < 0)
					newPosY -= topSideView;

				view.setCenter({ newPosX, newPosY });

				window.setView(view);
			}
			
			//Mouse Pressed
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (ImGui::GetIO().WantCaptureMouse)
					continue;
				
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
					isLeftMouseHeld = true;

				if (mouseButtonPressed->button == sf::Mouse::Button::Right)
					isRightMouseHeld = true;
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

				if (keyPressed->scancode == sf::Keyboard::Scan::H)
				{
					view.setCenter({ Settings::gridSizeInPixels / 2, Settings::gridSizeInPixels / 2 });
					view.setSize({ Settings::gridSizeInPixels, Settings::gridSizeInPixels });
				}

				if (keyPressed->scancode == sf::Keyboard::Scan::W)
					isUpHeld = true;

				if (keyPressed->scancode == sf::Keyboard::Scan::A)
					isLeftHeld = true;

				if (keyPressed->scancode == sf::Keyboard::Scan::S)
					isDownHeld = true;

				if (keyPressed->scancode == sf::Keyboard::Scan::D)
					isRightHeld = true;
			}

			if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
			{
				if (keyReleased->scancode == sf::Keyboard::Scan::W)
					isUpHeld = false;

				if (keyReleased->scancode == sf::Keyboard::Scan::A)
					isLeftHeld = false;

				if (keyReleased->scancode == sf::Keyboard::Scan::S)
					isDownHeld = false;

				if (keyReleased->scancode == sf::Keyboard::Scan::D)
					isRightHeld = false;
			}
		}
		
		//UPDATE

		if (isUpHeld && view.getCenter().y - (view.getSize().y / 2) > 0)
		{
			view.move({ 0,-5.f });
		}
		if (isDownHeld && view.getCenter().y + (view.getSize().y / 2) < Settings::gridSizeInPixels)
		{
			view.move({ 0, 5.f });
		}
		if (isRightHeld && view.getCenter().x + (view.getSize().x / 2) < Settings::gridSizeInPixels)
		{
			view.move({ 5.f, 0 });
		}
		if (isLeftHeld && view.getCenter().x - (view.getSize().x / 2) > 0)
		{
			view.move({ -5.f, 0 });
		}

		auto mappedMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
		mPos = static_cast<sf::Vector2i>(mappedMouse) / Settings::cellSize;

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

		window.setView(view);

		board.drawCellsVertices();

		board.drawGrid();

		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();
}