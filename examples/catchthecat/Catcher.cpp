#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) 
{
  _path = generatePath(world);

  if (!_path.empty())
  {
    _isRandomState = false;
    return _path.front();
  }
  else
  {
    auto side = world->getWorldSideSize() / 2;

    if (!_isRandomState)
    {
        _isRandomState = true;

        //Clear the cahce and add all unblocked points in the world to the cahce
        _randomPointCahce.clear();

        
        for (int y = -side; y <= side; y++)
        {
            for (int x = -side; x <= side; x++)
            {
                Point2D current(x, y);
                if (world->getContent(current)) continue;
                _randomPointCahce.insert(current);
            }
        }
    }

    //Scary!
    while (true) 
    {
      printf("Choosing random catcher position\n");
      auto cat = world->getCat();
      /*Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
      if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;*/
      Point2D next = *std::next(_randomPointCahce.begin(), Random::Range(0, _randomPointCahce.size() - 1));
      if (next == cat) continue;

      _randomPointCahce.extract(next);
      return next;
    }
  }
}
