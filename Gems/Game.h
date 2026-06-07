#pragma once
#ifndef GEMS_GAME_H
#define GEMS_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <vector>

#include "Constants.h"
#include "Gem.h"

class GemsGame {
 private:
  std::vector<std::vector<std::unique_ptr<Gem>>> grid;
  std::mt19937 rng;

  GemFabric fabric;

  void initGrid();
  void spavnBonus(int x, GemColor sourceColor);
  int cntBonus();
  bool checkMatches(bool remove);
  void dropGems();

 public:
  GemsGame();
  void swapGems(int x1, int y1, int x2, int y2);
  void update(float dt);
  void draw(sf::RenderWindow& window);
};

#endif