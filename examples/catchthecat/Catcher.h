#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"

#include <unordered_set>

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;

private:
  std::unordered_set<Point2D> _randomPointCahce;
  bool _isRandomState = false;
};

#endif  // CATCHER_H
