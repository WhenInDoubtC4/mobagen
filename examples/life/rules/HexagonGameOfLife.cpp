//
// Created by atolstenko on 2/9/2023.
//

#include "HexagonGameOfLife.h"
void HexagonGameOfLife::Step(World& world) 
{
	//Rules source: https://arunarjunakani.github.io/HexagonalGameOfLife/

	for (int x = 0; x < world.SideSize(); x++)
	{
		for (int y = 0; y < world.SideSize(); y++)
		{
            Point2D current(x, y);

			int liveNeighbors = CountNeighbors(world, current);

			//Live cell
			if (world.Get(current))
			{
				//Each cell with one or no neighbors dies from isolation.
				if (liveNeighbors <= 1) world.SetNext(current, false);

				//Each cell with three or more neighbors dies from overpopulation.
				if (liveNeighbors >= 3) world.SetNext(current, false);

				//Only the cells with two neighbors survives.
                if (liveNeighbors == 2) world.SetNext(current, true);
			}
			//Each cell with two neighbors revives.
            else if (liveNeighbors == 2) world.SetNext(current, true);
		}
	}
}

int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) 
{ 
	//Hexagons have 6 neighbors: 2 top/bottom, 1 left/right

	int liveNeighbors = 0;

	int lineOffset = point.y % 2;

	liveNeighbors += world.Get(Point2D(point.x - 1 + lineOffset, point.y - 1)); //Top left
	liveNeighbors += world.Get(Point2D(point.x + lineOffset, point.y - 1));  //Top right 
	liveNeighbors += world.Get(Point2D(point.x - 1, point.y)); //Left
	liveNeighbors += world.Get(Point2D(point.x + 1, point.y)); //Right
	liveNeighbors += world.Get(Point2D(point.x - 1 + lineOffset, point.y + 1)); //Bottom left
	liveNeighbors += world.Get(Point2D(point.x + lineOffset, point.y + 1)); //Bottom right 

	return liveNeighbors; 
}
