#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) 
{
  _path = generatePath(world);

  if (!_path.empty())
  {
    return _path.front();
  }
  else
  {
    auto side = world->getWorldSideSize() / 2;

    //Scary!
    while (true) 
    {
      printf("Choosing random catcher position\n");
      auto cat = world->getCat();
      
      std::vector<Point2D> neighbors = {
          World::E(cat), 
          World::W(cat), 
          World::NE(cat), 
          World::NW(cat), 
          World::SE(cat), 
          World::SW(cat)
      };

      Point2D next = neighbors[Random::Range(0, 5)];
      if (!world->catCanMoveToPosition(next)) continue;

      return next;
    }
  }
}
