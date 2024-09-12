#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) 
{ 
	for (int x = 0; x < world.SideSize(); x++)
	{
		for (int y = 0; y < world.SideSize(); y++)
		{
            Point2D current(x, y);

            int liveNeighbors = CountNeighbors(world, current);

			//Is cell alive
            if (world.Get(current)) 
			{
              //Any live cell with fewer than two live neighbours dies, as if by underpopulation.
              if (liveNeighbors < 2) world.SetNext(current, false);

              //Any live cell with two or three live neighbours lives on to the next generation.
              if (liveNeighbors == 2 || liveNeighbors == 3) world.SetNext(current, true);

              //Any live cell with more than three live neighbours dies, as if by overpopulation
              if (liveNeighbors > 3) world.SetNext(current, false);
            }
            //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            else if (liveNeighbors == 3)  world.SetNext(current, true);
		}
	}
}

int JohnConway::CountNeighbors(World& world, Point2D point) 
{
    int liveNeighbors = 0;

	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			//Skip self
			if (dx == 0 && dy == 0) continue;

			liveNeighbors += world.Get(Point2D(point.x + dx, point.y + dy));
		}
	}
	
	return liveNeighbors;
}