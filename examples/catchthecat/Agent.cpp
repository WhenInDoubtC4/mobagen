#include "Agent.h"
#include "World.h"

using namespace std;

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  
  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  int sideOver2 = w->getWorldSideSize() / 2;

  while (!frontier.empty()) {
    // get the current from frontier
    Point2D current = frontier.front();

    // remove the current from frontierset
    frontierSet.extract(current);
    frontier.pop();

    // mark current as visited
    visited.emplace(current, true);

    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    std::vector<Point2D> visitableNeighbors = getVisitableNeighbors(w, current);

    // iterate over the neighs:
    for (const Point2D& neighbor : visitableNeighbors)
    {
      // for every neighbor set the cameFrom
      cameFrom.emplace(current, neighbor);

      // enqueue the neighbors to frontier and frontierset
      frontier.push(neighbor);
      frontierSet.insert(neighbor);

      // do this up to find a visitable border and break the loop
      if (abs(neighbor.x) == sideOver2 || abs(neighbor.y) == sideOver2)
      {
        borderExit = neighbor;
        break;
      }
    }
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  

  // if there isnt a reachable border, just return empty vector
  

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

  int sideOver2 = w->getWorldSideSize() / 2;

  Point2D left = current + Point2D::LEFT;
  Point2D right = current + Point2D::RIGHT;

  //Left
  if (current.x > -sideOver2 && isVisitable(w, left)) neighbors.push_back(left);

  //Right
  if (current.x < sideOver2 && isVisitable(w, right)) neighbors.push_back(right);

  //Top
  if (current.y > -sideOver2)
  {
    Point2D topLeft = current + Point2D::UP + Point2D::LEFT;
    Point2D topRight = current + Point2D::UP + Point2D::LEFT;

    //Top left
    if (current.x > -sideOver2 && isVisitable(w, topLeft)) neighbors.push_back(topLeft);

    //Top right
    if (current.x < sideOver2 && isVisitable(w, topRight)) neighbors.push_back(topRight);
  }

  //Bottom
  if (current.y < sideOver2)
  {
    Point2D bottomLeft = current + Point2D::DOWN + Point2D::LEFT;
    Point2D bottomRight = current + Point2D::DOWN + Point2D::RIGHT;

    //Bottom left
    if (current.x > -sideOver2 && isVisitable(w, bottomLeft)) neighbors.push_back(bottomLeft);

    //Bottom right
    if (current.x < sideOver2 && isVisitable(w, bottomRight)) neighbors.push_back(bottomRight);
  }

  return neighbors;
}
