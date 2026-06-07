#ifndef GEM_H
#define GEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>

enum class GemColor { RED, GREEN, BLUE, YELLOW, PURPLE, CYAN };

class Gem {
 protected:
  float visualX;
  float visualY;
  int targetX;
  int targetY;
  GemColor color;

 public:
  Gem(GemColor c);
  virtual ~Gem() = default;

  virtual void activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid,
                        int y, int x, std::mt19937& rng) = 0;
  virtual bool isBonus() const { return false; }
  virtual bool isEmpty() const { return false; }
  virtual sf::Color getColor() const;

  void setPosition(float vx, float vy);
  void setTarget(int tx, int ty);
  float getVisualX() const { return visualX; }
  float getVisualY() const { return visualY; }
  int getTargetX() const { return targetX; }
  int getTargetY() const { return targetY; }
  GemColor getGemColor() const { return color; }

  virtual void draw(sf::RenderWindow& window, int cellSize) const;
};

class SimpGem : public Gem {
 public:
  SimpGem(GemColor c);
  void activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid, int y,
                int x, std::mt19937& rng) override {}
};

class BonusT : public Gem {
 private:
  GemColor sourceColor;

 public:
  BonusT(GemColor source);
  void activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid, int y,
                int x, std::mt19937& rng) override;
  bool isBonus() const override { return true; }
  sf::Color getColor() const override;
  GemColor getSourceColor() const { return sourceColor; }
};

class BombBonus : public Gem {
 public:
  BombBonus();
  void activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid, int y,
                int x, std::mt19937& rng) override;
  bool isBonus() const override { return true; }
  sf::Color getColor() const override;
};

class EmptyCell : public Gem {
 public:
  EmptyCell();
  void activate(std::vector<std::vector<std::unique_ptr<Gem>>>& grid, int y,
                int x, std::mt19937& rng) override {}
  bool isEmpty() const override { return true; }
  sf::Color getColor() const override;
};

class GemFabric {
 private:
  std::mt19937& rng;

 public:
  GemFabric(std::mt19937& random);
  std::unique_ptr<Gem> createRandomGem();
  std::unique_ptr<Gem> createBonus(GemColor sourceColor);
  std::unique_ptr<Gem> createEmpty();
};

#endif