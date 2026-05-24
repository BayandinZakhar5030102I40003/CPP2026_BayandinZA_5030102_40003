#pragma once
#ifndef GEM_H
#define GEM_H

#include <SFML/Graphics.hpp>

enum GemType {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PURPLE,
  CYAN,
  BONUS_REPAINT,
  BONUS_BOMB,
  EMPTY
};

struct Gem {
  GemType type = EMPTY;
  GemType sourceColor = RED;
  float visualX = 0;
  float visualY = 0;
  int targetX = 0;
  int targetY = 0;
};

#endif