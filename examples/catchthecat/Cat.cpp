#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) 
{
  std::vector<Point2D> path = generatePath(world);
  if (!path.empty())
  {
    printf("Cat moving according to path\n");
	return path[path.size() - 2];
  }
  else
  {
    printf("No reachable border, cat moving randomly\n");
    auto rand = Random::Range(0, 5);
    auto pos = world->getCat();
    switch (rand) 
    {
    case 0:
        return World::NE(pos);
    case 1:
        return World::NW(pos);
    case 2:
        return World::E(pos);
    case 3:
        return World::W(pos);
    case 4:
        return World::SW(pos);
    case 5:
        return World::SE(pos);
    default:
        throw "random out of range";
    }
  }
}
