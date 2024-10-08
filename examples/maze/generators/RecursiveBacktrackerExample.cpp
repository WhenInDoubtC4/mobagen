#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
#include <functional>

//Nasty, but necessary
const std::function<void(World&, const Point2D&, const bool&)> INDEX_TO_NEIGHBOR[] = { &World::SetNorth, &World::SetEast, &World::SetSouth, &World::SetWest};

bool RecursiveBacktrackerExample::Step(World* w) {

  if (stack.empty())
  {
    //Add top left cell to stack
    auto sideOver2 = w->GetSize() / 2;
    stack.push_back(Point2D(-sideOver2, -sideOver2));

    //Did not modify the world (yet)
    return true;
  }

  //There's nothing more to generate
  if (stack.empty()) return false;

  Point2D& topCell = stack.back();
  w->SetNodeColor(topCell, Color32(255, 0, 0));
  visited[topCell.y][topCell.x] = true;

  std::vector<std::pair<Point2D, int>> visitableNeighbors = getNeighbors(w, topCell);

  if (!visitableNeighbors.empty())
  {
    std::pair<Point2D, int>& selectedNeighbor = visitableNeighbors[Random::Range(0, visitableNeighbors.size() - 1)];

    //Remove wall
    INDEX_TO_NEIGHBOR[selectedNeighbor.second](*w, topCell, false);

    //Add the neighbor to the stack
    w->SetNodeColor(selectedNeighbor.first, Color32(255, 255, 0));
    stack.push_back(selectedNeighbor.first);

    return true;
  }
  else
  {
    //Backtrack once (set color to black)
    w->SetNodeColor(topCell, Color32(0, 0, 0));
    stack.pop_back();
    return true;
  }
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<std::pair<Point2D, int>> RecursiveBacktrackerExample::getNeighbors(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<std::pair<Point2D, int>> visitables;

  //Up
  if (p.y > -sideOver2 && !visited[p.y - 1][p.x]) visitables.emplace_back(Point2D(p.x, p.y - 1), 0);

  //Right
  if (p.x < sideOver2 && !visited[p.y][p.x + 1]) visitables.emplace_back(Point2D(p.x + 1, p.y), 1);

  //Down
  if (p.y < sideOver2 && !visited[p.y + 1][p.x]) visitables.emplace_back(Point2D(p.x, p.y + 1), 2);

  //Left
  if (p.x > -sideOver2 && !visited[p.y][p.x - 1]) visitables.emplace_back(Point2D(p.x - 1, p.y), 3); 

  return visitables;
}
