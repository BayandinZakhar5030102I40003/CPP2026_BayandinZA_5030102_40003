#include <SFML/Graphics.hpp>
#include <optional>

#include "Constants.h"
#include "Game.h"

int main() {
  sf::RenderWindow window(
      sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "GEMS");
  window.setFramerateLimit(60);

  GemsGame game;
  int selectX = -1;
  int selectY = -1;
  bool hasSelect = false;
  sf::Clock clock;

  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      if (const auto* mousePressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
          int mouseX = mousePressed->position.x;
          int mouseY = mousePressed->position.y;
          int gridX = mouseX / CELL_SIZE;
          int gridY = mouseY / CELL_SIZE;

          if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
            if (!hasSelect) {
              selectX = gridX;
              selectY = gridY;
              hasSelect = true;
            } else {
              if ((std::abs(selectX - gridX) + std::abs(selectY - gridY)) ==
                  1) {
                game.swapGems(selectX, selectY, gridX, gridY);
              }
              hasSelect = false;
              selectX = -1;
              selectY = -1;
            }
          }
        }
      }
    }

    game.update(deltaTime);
    window.clear(sf::Color(30, 30, 30));
    game.draw(window);

    if (hasSelect && selectX >= 0 && selectY >= 0) {
      sf::RectangleShape selection(
          sf::Vector2f((float)CELL_SIZE, (float)CELL_SIZE));
      selection.setPosition(sf::Vector2f((float)(selectX * CELL_SIZE),
                                         (float)(selectY * CELL_SIZE)));
      selection.setFillColor(sf::Color(255, 255, 255, 80));
      selection.setOutlineThickness(3);
      selection.setOutlineColor(sf::Color::Yellow);
      window.draw(selection);
    }

    window.display();
  }

  return 0;
}