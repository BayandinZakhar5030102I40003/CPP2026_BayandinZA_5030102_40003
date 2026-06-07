#include "Gem.h"

#include <algorithm>
#include <cmath>

Gem::Gem(GemColor c)
    : color(c), visualX(0), visualY(0), targetX(0), targetY(0) {}

void Gem::setPosition(float vx, float vy) {
  visualX = vx;
  visualY = vy;
}

void Gem::setTarget(int tx, int ty) {
  targetX = tx;
  targetY = ty;
}

sf::Color Gem::getColor() const {
  switch (color) {
    case GemColor::RED:
      return sf::Color(255, 0, 0);
    case GemColor::GREEN:
      return sf::Color(0, 255, 0);
    case GemColor::BLUE:
      return sf::Color(0, 0, 255);
    case GemColor::YELLOW:
      return sf::Color(255, 255, 0);
    case GemColor::PURPLE:
      return sf::Color(255, 0, 255);
    case GemColor::CYAN:
      return sf::Color(50, 255, 255);
    default:
      return sf::Color(0, 0, 0);
  }
}

void Gem::draw(sf::RenderWindow& window, int cellSize) const {
  sf::RectangleShape cell(
      sf::Vector2f((float)(cellSize - 2), (float)(cellSize - 2)));
  cell.setPosition(
      sf::Vector2f(visualX * cellSize + 1, visualY * cellSize + 1));
  cell.setFillColor(getColor());

  cell.setOutlineThickness(1);
  cell.setOutlineColor(sf::Color(0, 0, 0));

  window.draw(cell);
}

SimpGem::SimpGem(GemColor c) : Gem(c) {}

BonusT::BonusT(GemColor source) : Gem(GemColor::RED), sourceColor(source) {}

void BonusT::activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid,
                      int y, int x, std::mt19937& rng) {
  GemColor color = sourceColor;
  std::vector<std::pair<int, int>> cells;

  for (int cy = 0; cy < grid.size(); cy++) {
    for (int cx = 0; cx < grid[0].size(); cx++) {
      if (cx == x && cy == y) continue;
      int dist = std::abs(cx - x) + std::abs(cy - y);
      if (dist <= 1) continue;
      if (!grid[cy][cx]->isEmpty() && !grid[cy][cx]->isBonus()) {
        cells.push_back({cy, cx});
      }
    }
  }

  std::shuffle(cells.begin(), cells.end(), rng);

  grid[y][x] = std::make_unique<SimpGem>(color);
  grid[y][x]->setPosition((float)x, (float)y);
  grid[y][x]->setTarget(x, y);

  int count = std::min(2, (int)cells.size());
  for (int i = 0; i < count; i++) {
    int cy = cells[i].first;
    int cx = cells[i].second;
    grid[cy][cx] = std::make_unique<SimpGem>(color);
    grid[cy][cx]->setPosition((float)cx, (float)cy);
    grid[cy][cx]->setTarget(cx, cy);
  }
}

sf::Color BonusT::getColor() const { return sf::Color(255, 255, 255); }

BombBonus::BombBonus() : Gem(GemColor::RED) {}

void BombBonus::activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid,
                         int y, int x, std::mt19937& rng) {
  std::vector<std::pair<int, int>> cells;

  for (int cy = 0; cy < grid.size(); cy++) {
    for (int cx = 0; cx < grid[0].size(); cx++) {
      if (!grid[cy][cx]->isEmpty() && !grid[cy][cx]->isBonus()) {
        cells.push_back({cy, cx});
      }
    }
  }

  std::shuffle(cells.begin(), cells.end(), rng);

  grid[y][x] = std::make_unique<EmptyCell>();

  int destroy = 0;
  for (auto& c : cells) {
    if (destroy >= 5) break;
    int cy = c.first;
    int cx = c.second;
    if (!grid[cy][cx]->isEmpty() && !grid[cy][cx]->isBonus()) {
      grid[cy][cx] = std::make_unique<EmptyCell>();
      destroy++;
    }
  }
}

sf::Color BombBonus::getColor() const { return sf::Color(0, 0, 0); }

EmptyCell::EmptyCell() : Gem(GemColor::RED) {}

sf::Color EmptyCell::getColor() const { return sf::Color(255, 255, 255); }

GemFabric::GemFabric(std::mt19937& random) : rng(random) {}

std::unique_ptr<Gem> GemFabric::createRandomGem() {
  GemColor color = static_cast<GemColor>(rng() % 6);
  return std::make_unique<SimpGem>(color);
}

std::unique_ptr<Gem> GemFabric::createBonus(GemColor sourceColor) {
  if (rng() % 2 == 0) {
    return std::make_unique<BonusT>(sourceColor);
  } else {
    return std::make_unique<BombBonus>();
  }
}

std::unique_ptr<Gem> GemFabric::createEmpty() {
  return std::make_unique<EmptyCell>();
}