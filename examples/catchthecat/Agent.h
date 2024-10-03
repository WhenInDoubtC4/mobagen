#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

class World;

class Agent {
public:
  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);

private:
  std::unordered_map<Point2D, bool> visited;  // use .at() to get data, if the element dont exist [] will give you wrong results

  std::queue<Point2D> frontier;             // to store next ones to visit

  std::unordered_set<Point2D> frontierSet;  // OPTIMIZATION to check faster if a point is in the queue

  bool checkVisitable(const Point2D& target);
  std::vector<Point2D> getVisitableNeighbors(World* w, const Point2D& current);
};

#endif  // AGENT_H
