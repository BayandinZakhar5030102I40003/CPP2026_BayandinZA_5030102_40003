#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <SFML/Graphics.hpp>

#include "Game.h"

class Game {
 private:
  sf::RenderWindow window;
  GemsGame game;
  sf::Clock clock;

  int selectX;
  int selectY;
  bool select;

  void matches();
  void click(int gridX, int gridY);
  void render();
  void draw();

 public:
  Game();
  void run();
};

#endif