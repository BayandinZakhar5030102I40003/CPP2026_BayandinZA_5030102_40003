#include "Game.h"

#include <algorithm>
#include <chrono>
#include <queue>
#include <set>

GemsGame::GemsGame()
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()),
      fabric(rng) {
  initGrid();
}

int GemsGame::cntBonus() {
  int cnt = 0;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (grid[y][x]->isBonus()) cnt++;
    }
  }
  return cnt;
}

void GemsGame::initGrid() {
  grid.resize(HEIGHT);
  for (int y = 0; y < HEIGHT; y++) {
    grid[y].resize(WIDTH);
    for (int x = 0; x < WIDTH; x++) {
      grid[y][x] = fabric.createRandomGem();
      grid[y][x]->setPosition((float)x, (float)y);
      grid[y][x]->setTarget(x, y);
    }
  }

  while (checkMatches(false)) {
    checkMatches(true);
  }
}

void GemsGame::spavnBonus(int x, GemColor sourceColor) {
  if (cntBonus() >= MAX_BONUSES) return;
  if (rng() % 100 >= 25) return;

  auto bonus = fabric.createBonus(sourceColor);
  bonus->setPosition((float)x, -2.0f);
  bonus->setTarget(x, 0);

  for (int y = HEIGHT - 1; y > 0; y--) {
    grid[y][x] = std::move(grid[y - 1][x]);
    grid[y][x]->setTarget(x, y);
  }

  grid[0][x] = std::move(bonus);
}

bool GemsGame::checkMatches(bool remove) {
  std::set<std::pair<int, int>> toRemove;
  std::vector<std::pair<int, int>> bonusesToActivate;
  std::vector<std::pair<int, GemColor>> bonusSpavns;

  std::vector<std::vector<bool>> visited(HEIGHT,
                                         std::vector<bool>(WIDTH, false));

  const int dx[] = {0, 0, -1, 1};
  const int dy[] = {-1, 1, 0, 0};

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (grid[y][x]->isEmpty() || grid[y][x]->isBonus()) {
        continue;
      }

      GemColor targetColor = grid[y][x]->getGemColor();

      for (int dir = 0; dir < 4; dir++) {
        std::vector<std::pair<int, int>> line;
        std::queue<std::pair<int, int>> q;
        std::vector<std::vector<bool>> lineVisited(
            HEIGHT, std::vector<bool>(WIDTH, false));

        q.push({y, x});
        lineVisited[y][x] = true;

        while (!q.empty()) {
          auto [cy, cx] = q.front();
          q.pop();

          line.push_back({cy, cx});

          int ny = cy + dy[dir];
          int nx = cx + dx[dir];

          if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH &&
              !lineVisited[ny][nx] && !grid[ny][nx]->isEmpty() &&
              !grid[ny][nx]->isBonus() &&
              grid[ny][nx]->getGemColor() == targetColor) {
            lineVisited[ny][nx] = true;
            q.push({ny, nx});
          }
        }

        if (line.size() >= 3) {
          for (auto& cell : line) {
            toRemove.insert(cell);
            visited[cell.first][cell.second] = true;
          }

          int firstY = line[0].first;
          int firstX = line[0].second;
          int lastY = line.back().first;
          int lastX = line.back().second;

          int beforeY = firstY - dy[dir];
          int beforeX = firstX - dx[dir];
          if (beforeY >= 0 && beforeY < HEIGHT && beforeX >= 0 &&
              beforeX < WIDTH && grid[beforeY][beforeX]->isBonus()) {
            bool alreadyAdded = false;
            for (auto& bonus : bonusesToActivate) {
              if (bonus.first == beforeY && bonus.second == beforeX) {
                alreadyAdded = true;
                break;
              }
            }
            if (!alreadyAdded) {
              bonusesToActivate.push_back({beforeY, beforeX});
            }
          }

          int afterY = lastY + dy[dir];
          int afterX = lastX + dx[dir];
          if (afterY >= 0 && afterY < HEIGHT && afterX >= 0 && afterX < WIDTH &&
              grid[afterY][afterX]->isBonus()) {
            bool alreadyAdded = false;
            for (auto& bonus : bonusesToActivate) {
              if (bonus.first == afterY && bonus.second == afterX) {
                alreadyAdded = true;
                break;
              }
            }
            if (!alreadyAdded) {
              bonusesToActivate.push_back({afterY, afterX});
            }
          }

          if (remove) {
            int middleIdx = line.size() / 2;
            bonusSpavns.push_back({line[middleIdx].second, targetColor});
          }
        }
      }
    }
  }

  if (!remove) return !toRemove.empty();
  if (toRemove.empty() && bonusesToActivate.empty()) return false;

  for (auto& pos : bonusesToActivate) {
    int y = pos.first;
    int x = pos.second;
    if (!grid[y][x]->isEmpty() && grid[y][x]->isBonus()) {
      grid[y][x]->activate(grid, y, x, rng);
    }
  }

  for (auto& pos : toRemove) {
    grid[pos.first][pos.second] = fabric.createEmpty();
  }

  for (auto& spavn : bonusSpavns) {
    spavnBonus(spavn.first, spavn.second);
  }

  dropGems();

  return true;
}

void GemsGame::dropGems() {
  for (int x = 0; x < WIDTH; x++) {
    std::vector<std::unique_ptr<Gem>> column;
    for (int y = HEIGHT - 1; y >= 0; y--) {
      if (!grid[y][x]->isEmpty()) {
        column.push_back(std::move(grid[y][x]));
      }
    }

    while (column.size() < HEIGHT) {
      auto gem = fabric.createRandomGem();
      gem->setPosition((float)x, -3.0f);
      gem->setTarget(x, 0);
      column.push_back(std::move(gem));
    }

    for (int i = 0; i < HEIGHT; i++) {
      int newY = HEIGHT - 1 - i;
      grid[newY][x] = std::move(column[i]);
      grid[newY][x]->setTarget(x, newY);
    }
  }
}

void GemsGame::swapGems(int x1, int y1, int x2, int y2) {
  if (x1 < 0 || x1 >= WIDTH || y1 < 0 || y1 >= HEIGHT || x2 < 0 ||
      x2 >= WIDTH || y2 < 0 || y2 >= HEIGHT)
    return;

  if (grid[y1][x1]->isBonus() || grid[y2][x2]->isBonus()) return;

  std::swap(grid[y1][x1], grid[y2][x2]);

  grid[y1][x1]->setTarget(x1, y1);
  grid[y2][x2]->setTarget(x2, y2);

  if (checkMatches(false)) {
    while (checkMatches(true)) {
    }
  } else {
    std::swap(grid[y1][x1], grid[y2][x2]);
    grid[y1][x1]->setTarget(x1, y1);
    grid[y2][x2]->setTarget(x2, y2);
  }
}

void GemsGame::update(float dt) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      float speed = 7;
      float vx = grid[y][x]->getVisualX();
      float vy = grid[y][x]->getVisualY();
      int tx = grid[y][x]->getTargetX();
      int ty = grid[y][x]->getTargetY();

      grid[y][x]->setPosition(vx + (tx - vx) * speed * dt,
                              vy + (ty - vy) * speed * dt);
    }
  }
}

void GemsGame::draw(sf::RenderWindow& window) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      grid[y][x]->draw(window, CELL_SIZE);
    }
  }
}