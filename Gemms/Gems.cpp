#include <algorithm>
#include <chrono>
#include <set>

#include "Constants.h"
#include "Game.h"

GemsGame::GemsGame()
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
  initGrid();
}

GemType GemsGame::randomGem() { return static_cast<GemType>(rng() % 6); }

bool GemsGame::isBonus(GemType type) {
  return type == BONUS_REPAINT || type == BONUS_BOMB;
}

int GemsGame::cntBonus() {
  int cnt = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (isBonus(grid[y][x].type)) cnt++;
    }
  }
  return cnt;
}

void GemsGame::initGrid() {
  grid.assign(HEIGHT, std::vector<Gem>(WIDTH));

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      grid[y][x].type = randomGem();
      grid[y][x].visualX = (float)x;
      grid[y][x].visualY = (float)y;
      grid[y][x].targetX = x;
      grid[y][x].targetY = y;
    }
  }

  while (checkMatches(true)) {
  }
}

void GemsGame::spavnBonus(int x, GemType sourceColor) {
  if (cntBonus() >= MAX_BONUSES) return;
  if (rng() % 100 >= BONUS_SPAVN_PERCENT) return;

  for (int y = 0; y < HEIGHT; y++) {
    if (grid[y][x].type == EMPTY) {
      Gem bonus;
      bonus.type = (rng() % 2 == 0) ? BONUS_REPAINT : BONUS_BOMB;
      bonus.sourceColor = sourceColor;
      bonus.visualX = (float)x;
      bonus.visualY = -3.0f;
      bonus.targetX = x;
      bonus.targetY = y;
      grid[y][x] = bonus;
      return;
    }
  }
}

void GemsGame::applyRepaintBonus(int cntrY, int cntrX) {
  GemType color = grid[cntrY][cntrX].sourceColor;
  std::vector<std::pair<int, int>> cells;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (x == cntrX && y == cntrY) continue;
      int dist = abs(x - cntrX) + abs(y - cntrY);
      if (dist <= 1) continue;
      if (grid[y][x].type != EMPTY && !isBonus(grid[y][x].type)) {
        cells.push_back({y, x});
      }
    }
  }

  std::shuffle(cells.begin(), cells.end(), rng);
  grid[cntrY][cntrX].type = color;

  for (int i = 0; i < std::min(2, (int)cells.size()); i++) {
    int y = cells[i].first;
    int x = cells[i].second;
    grid[y][x].type = color;
  }
}

void GemsGame::applyBonus(int cntrY, int cntrX) {
  std::vector<std::pair<int, int>> cells;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (grid[y][x].type != EMPTY) {
        cells.push_back({y, x});
      }
    }
  }

  std::shuffle(cells.begin(), cells.end(), rng);
  grid[cntrY][cntrX].type = EMPTY;
  int destroyed = 1;

  for (auto& c : cells) {
    if (destroyed >= 5) break;
    int y = c.first;
    int x = c.second;
    if (y == cntrY && x == cntrX) continue;
    grid[y][x].type = EMPTY;
    destroyed++;
  }
}

bool GemsGame::checkMatches(bool remove) {
  std::set<std::pair<int, int>> toRemove;
  std::vector<std::pair<int, int>> bonusesToActivate;
  std::vector<std::pair<int, GemType>> bonusSpavns;

  for (int y = 0; y < HEIGHT; y++) {
    int count = 1;
    for (int x = 1; x < WIDTH; x++) {
      if (grid[y][x].type != EMPTY && !isBonus(grid[y][x].type) &&
          grid[y][x].type == grid[y][x - 1].type) {
        count++;
      } else {
        if (count >= 3) {
          GemType color = grid[y][x - 1].type;
          for (int k = 0; k < count; k++) {
            int xx = x - 1 - k;
            toRemove.insert({y, xx});
            if (xx > 0 && isBonus(grid[y][xx - 1].type)) {
              bonusesToActivate.push_back({y, xx - 1});
            }
            if (xx < WIDTH - 1 && isBonus(grid[y][xx + 1].type)) {
              bonusesToActivate.push_back({y, xx + 1});
            }
          }
          if (remove) {
            bonusSpavns.push_back({x - 1, color});
          }
        }
        count = 1;
      }
    }
    if (count >= 3) {
      GemType color = grid[y][WIDTH - 1].type;
      for (int k = 0; k < count; k++) {
        int xx = WIDTH - 1 - k;
        toRemove.insert({y, xx});
      }
      if (remove) {
        bonusSpavns.push_back({WIDTH - 1, color});
      }
    }
  }

  for (int x = 0; x < WIDTH; x++) {
    int count = 1;
    for (int y = 1; y < HEIGHT; y++) {
      if (grid[y][x].type != EMPTY && !isBonus(grid[y][x].type) &&
          grid[y][x].type == grid[y - 1][x].type) {
        count++;
      } else {
        if (count >= 3) {
          GemType color = grid[y - 1][x].type;
          for (int k = 0; k < count; k++) {
            int yy = y - 1 - k;
            toRemove.insert({yy, x});
            if (yy > 0 && isBonus(grid[yy - 1][x].type)) {
              bonusesToActivate.push_back({yy - 1, x});
            }
            if (yy < HEIGHT - 1 && isBonus(grid[yy + 1][x].type)) {
              bonusesToActivate.push_back({yy + 1, x});
            }
          }
          if (remove) {
            bonusSpavns.push_back({x, color});
          }
        }
        count = 1;
      }
    }
    if (count >= 3) {
      GemType color = grid[HEIGHT - 1][x].type;
      for (int k = 0; k < count; k++) {
        int yy = HEIGHT - 1 - k;
        toRemove.insert({yy, x});
      }
      if (remove) {
        bonusSpavns.push_back({x, color});
      }
    }
  }

  if (!remove) return !toRemove.empty();
  if (toRemove.empty()) return false;

  for (auto& pos : bonusesToActivate) {
    int y = pos.first;
    int x = pos.second;
    if (grid[y][x].type == BONUS_REPAINT) {
      applyRepaintBonus(y, x);
    } else if (grid[y][x].type == BONUS_BOMB) {
      applyBonus(y, x);
    }
  }

  for (auto& pos : toRemove) {
    grid[pos.first][pos.second].type = EMPTY;
  }

  for (auto& req : bonusSpavns) {
    spavnBonus(req.first, req.second);
  }

  dropGems();
  return true;
}

void GemsGame::dropGems() {
  for (int x = 0; x < WIDTH; x++) {
    std::vector<Gem> column;
    for (int y = HEIGHT - 1; y >= 0; y--) {
      if (grid[y][x].type != EMPTY) {
        column.push_back(grid[y][x]);
      }
    }
    while (column.size() < HEIGHT) {
      Gem gem;
      gem.type = randomGem();
      gem.visualX = (float)x;
      gem.visualY = -3.0f;
      gem.targetX = x;
      gem.targetY = 0;
      column.push_back(gem);
    }
    for (int i = 0; i < HEIGHT; i++) {
      int newY = HEIGHT - 1 - i;
      grid[newY][x] = column[i];
      grid[newY][x].targetX = x;
      grid[newY][x].targetY = newY;
    }
  }
}

void GemsGame::swapGems(int x1, int y1, int x2, int y2) {
  if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT || x2 < 0 ||
      x2 >= WIDTH || y2 < 0 || y2 >= HEIGHT)
    return;

  if (isBonus(grid[y1][x1].type) || isBonus(grid[y2][x2].type)) return;

  std::swap(grid[y1][x1], grid[y2][x2]);

  grid[y1][x1].targetX = x1;
  grid[y1][x1].targetY = y1;
  grid[y2][x2].targetX = x2;
  grid[y2][x2].targetY = y2;

  if (checkMatches(false)) {
    while (checkMatches(true)) {
    }
  } else {
    std::swap(grid[y1][x1], grid[y2][x2]);
    grid[y1][x1].targetX = x1;
    grid[y1][x1].targetY = y1;
    grid[y2][x2].targetX = x2;
    grid[y2][x2].targetY = y2;
  }
}

void GemsGame::update(float dt) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      float speed = 10;
      grid[y][x].visualX +=
          (grid[y][x].targetX - grid[y][x].visualX) * speed * dt;
      grid[y][x].visualY +=
          (grid[y][x].targetY - grid[y][x].visualY) * speed * dt;
    }
  }
}

void GemsGame::draw(sf::RenderWindow& window) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      sf::RectangleShape cell(
          sf::Vector2f((float)(CELL_SIZE - 2), (float)(CELL_SIZE - 2)));
      cell.setPosition(sf::Vector2f(grid[y][x].visualX * CELL_SIZE + 1,
                                    grid[y][x].visualY * CELL_SIZE + 1));
      cell.setFillColor(GEM_COLORS.at(grid[y][x].type));

      cell.setOutlineThickness(1);
      cell.setOutlineColor(sf::Color(80, 80, 80));

      window.draw(cell);
    }
  }
}