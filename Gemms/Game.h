#pragma once
#ifndef GEMS_GAME_H
#define GEMS_GAME_H

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

#include "Gem.h"

class GemsGame {
 private:
  std::vector<std::vector<Gem>> grid;
  std::mt19937 rng;

  GemType randomGem();
  bool isBonus(GemType type);
  int cntBonus();
  void initGrid();
  void spavnBonus(int x, GemType color);
  void applyRepaintBonus(int cntrY, int cntrX);
  void applyBonus(int cntrY, int cntrX);
  bool checkMatches(bool remove = false);
  void dropGems();

 public:
  GemsGame();
  void swapGems(int x1, int y1, int x2, int y2);
  void update(float dt);
  void draw(sf::RenderWindow& window);
};

#endif
