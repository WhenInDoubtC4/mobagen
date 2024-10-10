#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) 
{
  _path = generatePath(world);
  if (!_path.empty())
  {
    if (_path.size() > 2)
      return _path[_path.size() - 2];
    else
      return _path.back();
  }
  else
  {
    auto pos = world->getCat();
    Point2D result = Point2D::INFINITE;

    do
    {
      auto rand = Random::Range(0, 5);
      switch (rand) {
        case 0:
          result = World::NE(pos);
          break;
        case 1:
          result = World::NW(pos);
          break;
        case 2:
          result = World::E(pos);
          break;
        case 3:
          result = World::W(pos);
          break;
        case 4:
          result = World::SW(pos);
          break;
        case 5:
          result = World::SE(pos);
          break;
        default:
          throw "random out of range";
      }
      printf("RAND: %i\n", rand);
    } while (result == Point2D::INFINITE || !world->catCanMoveToPosition(result));

    return result;
  }
}
