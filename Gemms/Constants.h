#pragma once
#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <SFML/Graphics.hpp>
#include <map>

#include "Gem.h"

const int WIDTH = 20;
const int HEIGHT = 10;
const int CELL_SIZE = 75;
const int WINDOW_WIDTH = WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = HEIGHT * CELL_SIZE;
const int MAX_BONUSES = 10;
const int BONUS_SPAVN_PERCENT = 25;

const std::map<GemType, sf::Color> GEM_COLORS = {
    {RED, sf::Color(255, 0, 0)},
    {GREEN, sf::Color(0, 255, 0)},
    {BLUE, sf::Color(0, 0, 255)},
    {YELLOW, sf::Color(255, 255, 0)},
    {PURPLE, sf::Color(255, 0, 255)},
    {CYAN, sf::Color(50, 255, 255)},
    {BONUS_REPAINT, sf::Color(255, 255, 255)},
    {BONUS_BOMB, sf::Color(0, 0, 0)},
    {EMPTY, sf::Color(30, 30, 30)},
};

#endif