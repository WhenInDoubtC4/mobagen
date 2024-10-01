#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>
#include <functional>

// Nasty, but necessary
const std::function<void(World&, const Point2D&, const bool&)> INDEX_TO_NEIGHBOR[]
    = {&World::SetNorth, &World::SetEast, &World::SetSouth, &World::SetWest};

bool HuntAndKillExample::Step(World* w) 
{
  auto sideOver2 = w->GetSize() / 2;

  static bool huntMode = false;
  static int huntRowIndex = -sideOver2;

  if (stack.empty() && !huntMode)
  {
    Point2D start = randomStartPoint(w);
    stack.push_back(start);
  }

  if (huntMode)
  {
    if (huntRowIndex >= sideOver2 + 1) return false;

    bool found = false;

    //Set row color
    for (int i = -sideOver2; i <= sideOver2; i++)
    {
      if (huntRowIndex > -sideOver2) 
      {
        if (!visited[huntRowIndex - 1][i]) w->SetNodeColor(Point2D(i, huntRowIndex - 1), Color::DarkGray);
      }
      
      if (visited[huntRowIndex][i]) continue;
      w->SetNodeColor(Point2D(i, huntRowIndex), Color32(255, 255, 0));
      if (found) continue;

      std::vector<std::pair<Point2D, int>> visitedNeighbors = getNeighbors(w, Point2D(i, huntRowIndex), false);
      if (!visitedNeighbors.empty())
      {
        std::pair<Point2D, int> selected = visitedNeighbors[Random::Range(0, visitedNeighbors.size() - 1)];

        stack.push_back(selected.first);
        w->SetNodeColor(selected.first, Color32(255, 255, 0));
        INDEX_TO_NEIGHBOR[selected.second](*w, selected.first, false);
        huntMode = false;

        found = true;
      }
    }

    if (found)
    {
      for (int i = -sideOver2; i <= sideOver2; i++)
      {
        if (!visited[huntRowIndex][i]) w->SetNodeColor(Point2D(i, huntRowIndex), Color::DarkGray);
      }
    }

    huntRowIndex++;

    return true;
  }

  if (stack.empty()) return false;

  //Get top cell and mark as visited
  Point2D& topCell = stack.back();
  w->SetNodeColor(topCell, Color32(0, 0, 0));
  visited[topCell.y][topCell.x] = true;

  std::vector<std::pair<Point2D, int>> visitableNeighbors = getNeighbors(w, topCell);

  if (!visitableNeighbors.empty())
  {
    //Reduce, reuse, recycle...

    std::pair<Point2D, int>& selectedNeighbor = visitableNeighbors[Random::Range(0, visitableNeighbors.size() - 1)];

    // Remove wall
    INDEX_TO_NEIGHBOR[selectedNeighbor.second](*w, topCell, false);

    // Add the neighbor to the stack
    w->SetNodeColor(selectedNeighbor.first, Color32(255, 255, 0));
    stack.push_back(selectedNeighbor.first);

    return true;
  }
  else
  {
    huntMode = true;
    huntRowIndex = -sideOver2;
    stack.clear();
    return true;
  }

  return false;
}
void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}
Point2D HuntAndKillExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  return {Random::Range(-sideOver2, sideOver2), Random::Range(-sideOver2, sideOver2)};
}

std::vector<std::pair<Point2D, int>> HuntAndKillExample::getNeighbors(World* w, const Point2D& p, bool lookForVisitable) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<std::pair<Point2D, int>> visitables;

  //Reduce, reuse, recycle -- from RecursiveBacktrackerExample.cpp

  // Up
  if (p.y > -sideOver2 && !(!visited[p.y - 1][p.x] ^ lookForVisitable)) visitables.emplace_back(Point2D(p.x, p.y - 1), 0);

  // Right
  if (p.x < sideOver2 && !(!visited[p.y][p.x + 1] ^ lookForVisitable)) visitables.emplace_back(Point2D(p.x + 1, p.y), 1);

  // Down
  if (p.y < sideOver2 && !(!visited[p.y + 1][p.x] ^ lookForVisitable)) visitables.emplace_back(Point2D(p.x, p.y + 1), 2);

  // Left
  if (p.x > -sideOver2 && !(!visited[p.y][p.x - 1] ^ lookForVisitable)) visitables.emplace_back(Point2D(p.x - 1, p.y), 3); 

  return visitables;
}