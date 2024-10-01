#include "PrimExample.h"
#include "../World.h"
#include "Random.h"
#include <functional>
#include <utility>

const std::function<void(World&, const Point2D&, const bool&)> INDEX_TO_NEIGHBOR[] = {&World::SetNorth, &World::SetEast, &World::SetSouth, &World::SetWest};

bool PrimExample::Step(World* w) {
  int sideOver2 = w->GetSize() / 2;

  if (!initialized)
  {
    initialized = true;

    //Choose a point randomly
    int rx = Random::Range(-sideOver2, sideOver2);
    int ry = Random::Range(-sideOver2, sideOver2);
    Point2D startPoint(rx, ry);

    previous = startPoint;
    visited.insert(startPoint);
    toBeVisited.insert_range(getVisitables(w, startPoint));

    w->SetNodeColor(startPoint, Color32(255, 255, 0));

    return true;
  }

  if (toBeVisited.empty())
  {
    w->SetNodeColor(previous, Color32(0, 0, 0));
    return false;
  }

  //Randomly choose a point from the frontier
  int nextPointIndex = Random::Range(0, toBeVisited.size() - 1);
  Point2D nextPoint = *std::next(toBeVisited.begin(), nextPointIndex);
  
  //Mark its visited and add any new(!) neighbors to be visited
  toBeVisited.erase(nextPoint);
  visited.insert(nextPoint);
  toBeVisited.insert_range(getVisitables(w, nextPoint));

  updateVisitedNeighbors(w, nextPoint);

  w->SetNodeColor(previous, Color32(0, 0, 0));
  w->SetNodeColor(nextPoint, Color32(255, 255, 0));
  previous = nextPoint;

  return true;
}
void PrimExample::Clear(World* world) {
  visited.clear();
  toBeVisited.clear();
  initialized = false;
}

std::vector<Point2D> PrimExample::getVisitables(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  for (const Point2D& delta : deltas) 
  {
    Point2D next = p + delta;

    if (next.x < -sideOver2 || next.y < -sideOver2 || next.x > sideOver2 || next.y > sideOver2) continue;

    if (!visited.contains(next)) 
    {
      visitables.push_back(next);
      w->SetNodeColor(next, Color32(255, 0, 0));
    }
  }

  return visitables;
}

void PrimExample::updateVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::RIGHT, Point2D::DOWN, Point2D::LEFT};
  std::vector<int> neighbors;

  for (int i = 0; i < deltas.size(); i++)
  {
    Point2D delta = deltas[i];

    if (visited.contains(p + delta)) neighbors.push_back(i);
  }

  //If there are multiple, choose one randomly
  int selectedIndex = neighbors[Random::Range(0, neighbors.size() - 1)];

  //Remove wall
  INDEX_TO_NEIGHBOR[selectedIndex](*w, p, false);
}
