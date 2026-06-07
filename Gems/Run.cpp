#include "Run.h"

#include <cmath>
#include <optional>

#include "Constants.h"

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "GEMS"),
      selectX(-1),
      selectY(-1),
      select(false) {
  window.setFramerateLimit(60);
}

void Game::run() {
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    matches();
    game.update(deltaTime);
    render();
  }
}

void Game::matches() {
  while (const std::optional<sf::Event> event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }

    if (const auto* mouseclick =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      if (mouseclick->button == sf::Mouse::Button::Left) {
        int gridX = mouseclick->position.x / CELL_SIZE;
        int gridY = mouseclick->position.y / CELL_SIZE;

        if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
          click(gridX, gridY);
        }
      }
    }
  }
}

void Game::click(int gridX, int gridY) {
  if (!select) {
    selectX = gridX;
    selectY = gridY;
    select = true;
  } else {
    if ((std::abs(selectX - gridX) + std::abs(selectY - gridY)) == 1) {
      game.swapGems(selectX, selectY, gridX, gridY);
    }
    select = false;
    selectX = -1;
    selectY = -1;
  }
}

void Game::render() {
  window.clear(sf::Color(30, 30, 30));
  game.draw(window);
  draw();
  window.display();
}

void Game::draw() {
  if (select && selectX >= 0 && selectY >= 0) {
    sf::RectangleShape selection(
        sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));
    selection.setPosition(sf::Vector2f((float)(selectX * CELL_SIZE),
                                       (float)(selectY * CELL_SIZE)));
    selection.setFillColor(sf::Color(255, 255, 255, 80));
    selection.setOutlineThickness(3);
    selection.setOutlineColor(sf::Color::Yellow);
    window.draw(selection);
  }
}