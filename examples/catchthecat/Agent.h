#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <utility>
#include <functional>

typedef std::pair<Point2D, int> PQ_ITEM;

class World;

class Agent {
public:
  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);

  std::vector<Point2D>& getPath() { return _path; };

protected:
  std::vector<Point2D> _path;

private:
  std::unordered_map<Point2D, bool> visited;  // use .at() to get data, if the element dont exist [] will give you wrong results

  std::priority_queue<PQ_ITEM, std::vector<PQ_ITEM>, decltype([](PQ_ITEM a, PQ_ITEM b) -> bool { return a.second > b.second; })>
      frontier;  // to store next ones to visit

  std::unordered_set<Point2D> frontierSet;  // OPTIMIZATION to check faster if a point is in the queue

  bool isVisitable(World* w, const Point2D& target);
  std::vector<Point2D> getVisitableNeighbors(World* w, const Point2D& current);
  int heuristic(const Point2D& current, int sideSize);
};

#endif  // AGENT_H
