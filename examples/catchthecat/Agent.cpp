#include "Agent.h"
#include "World.h"

using namespace std;

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  unordered_map<Point2D, int> costSoFar;

  //Clear frontier and visited map
  frontier = {};
  frontierSet.clear();
  visited.clear();

  // bootstrap state
  auto catPos = w->getCat();
  printf("Cat x=%i y=%i\n", catPos.x, catPos.y);
  frontier.emplace(catPos, 0);
  costSoFar.emplace(catPos, 0);
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  int sideOver2 = w->getWorldSideSize() / 2;

  bool breakLoop = false;
  while (!frontier.empty() && !breakLoop) 
  {
    // get the current from frontier
    Point2D current = frontier.top().first;

    // remove the current from frontierset
    frontierSet.extract(current);
    frontier.pop();

    // mark current as visited
    visited.emplace(current, true);

    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    std::vector<Point2D> visitableNeighbors = getVisitableNeighbors(w, current);

    printf("Current x=%i y=%i\n", current.x, current.y);

    // iterate over the neighs:
    printf("Neighbors: \n");
    for (const Point2D& neighbor : visitableNeighbors)
    {
      printf("(x=%i y=%i), ", neighbor.x, neighbor.y);

      int newCost = costSoFar.at(current) + 1;
      if (!costSoFar.contains(neighbor) || newCost < costSoFar.at(neighbor))
      {
        costSoFar[neighbor] = newCost;
        int priority;
        priority = newCost + heuristic(borderExit, sideOver2);
        
        //enqueue the neighbors to frontier and frontierset
        frontier.emplace(neighbor, priority);
        frontierSet.insert(neighbor);

        // for every neighbor set the cameFrom
        cameFrom.emplace(neighbor, current);

        // do this up to find a visitable border and break the loop
        if (abs(neighbor.x) == sideOver2 || abs(neighbor.y) == sideOver2) 
        {
          printf("Border found (%i, %i)\n", neighbor.x, neighbor.y);
          borderExit = neighbor;
          breakLoop = true;
          break;
        }
      }
    }
  }
  printf("\n");

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  if (borderExit != Point2D::INFINITE)
  {
    std::vector<Point2D> path;

    Point2D current = borderExit;
    while (current != w->getCat())
    {
      printf("(%i, %i) -> (%i, %i)\n", current.x, current.y, cameFrom[current].x, cameFrom[current].y);

      path.push_back(current);
      current = cameFrom[current];
    }
    path.push_back(w->getCat());
    printf("end\n");


    return path;
  }
  else
  {
    // if there isnt a reachable border, just return empty vector
    return std::vector<Point2D>();
  }
 
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move
  return vector<Point2D>();
}

bool Agent::isVisitable(World* w, const Point2D& target) 
{
  return !visited.contains(target) //not visited
      && w->getCat() != target  //not cat
      && !w->getContent(target)  //not block
      && !frontierSet.contains(target); //not in the queue
}

std::vector<Point2D> Agent::getVisitableNeighbors(World* w, const Point2D& current) {
  std::vector<Point2D> neighbors;

  //TODO: Need to add hex offset for top/bottom, for odd lines

  int sideOver2 = w->getWorldSideSize() / 2;

  bool evenLine = current.y % 2 == 0;

  Point2D left = current + Point2D::LEFT;
  Point2D right = current + Point2D::RIGHT;

  //Left
  if (current.x > -sideOver2 && isVisitable(w, left)) neighbors.push_back(left);

  //Right
  if (current.x < sideOver2 && isVisitable(w, right)) neighbors.push_back(right);

  //Top
  if (current.y > -sideOver2)
  {
    Point2D topLeft = current + Point2D::UP;
    Point2D topRight = current + Point2D::UP;

    if (evenLine)
      topLeft += Point2D::LEFT;
    else
      topRight += Point2D::RIGHT;

    //Top left
    if (current.x > -sideOver2 && isVisitable(w, topLeft)) neighbors.push_back(topLeft);

    //Top right
    if (current.x < sideOver2 && isVisitable(w, topRight)) neighbors.push_back(topRight);
  }

  //Bottom
  if (current.y < sideOver2)
  {
    Point2D bottomLeft = current + Point2D::DOWN;
    Point2D bottomRight = current + Point2D::DOWN;

    if (evenLine)
      bottomLeft += Point2D::LEFT;
    else
      bottomRight += Point2D::RIGHT;

    //Bottom left
    if (current.x > -sideOver2 && isVisitable(w, bottomLeft)) neighbors.push_back(bottomLeft);

    //Bottom right
    if (current.x < sideOver2 && isVisitable(w, bottomRight)) neighbors.push_back(bottomRight);
  }

  return neighbors;
}

int Agent::heuristic(const Point2D& current, int sideSize) 
{ 
    //Manhattan distance to closest border
    return abs(abs(current.x) - sideSize) + abs(abs(current.y) - sideSize); 
}
