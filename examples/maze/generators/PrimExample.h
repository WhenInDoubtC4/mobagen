#ifndef MOBAGEN_EXAMPLES_MAZE_GENERATORS_PRIMEXAMPLE_H_
#define MOBAGEN_EXAMPLES_MAZE_GENERATORS_PRIMEXAMPLE_H_

#include <vector>
#include "../MazeGeneratorBase.h"
#include "math/Point2D.h"
#include <map>
#include <unordered_set>

class PrimExample : public MazeGeneratorBase {
private:
  std::unordered_set<Point2D> toBeVisited;
  std::unordered_set<Point2D> visited;
  bool initialized = false;
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);
  void updateVisitedNeighbors(World* w, const Point2D& p);
  Point2D previous;

public:
  PrimExample() = default;
  std::string GetName() override { return "Prim"; };
  bool Step(World* world) override;
  void Clear(World* world) override;
};

#endif  // MOBAGEN_EXAMPLES_MAZE_GENERATORS_PRIMEXAMPLE_H_
