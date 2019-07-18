#include "UniformGrid.h"

UniformGrid::UniformGrid()
{
	maxR = 0.0;
}

UniformGrid::~UniformGrid()
{
}

//Handles setting the size of the object structure
void UniformGrid::setobj(int objCount)
{
	object = new objects[(objCount)];
	objCounter = (objCount + 1);
}

//Handles storing the objects read in from the file
float UniformGrid::setObject(float x, float y, float z, float r, int objCount)
{
	//Set the values for the objects
	this->object[objCount].x = x;
	this->object[objCount].y = y;
	this->object[objCount].z = z;
	this->object[objCount].r = r;

	//Store the largest radius
	if (r > maxR || maxR == 10)
		maxR = r;

	return maxR;
}

//Handles setting the grid values and cells
void UniformGrid::setGrid(int screenX, int screenY, int screenZ, float rad)
{
	//Holds the size of the cells - atleast as big as the largest object
	int cellSize = round((rad * 2));

	//Ensure the size of the cells is larger than the largest object
	if (cellSize < (rad * 2))
		cellSize++;

	//Calculate the number of cells to be made in the grid
	cellNumX = screenX / cellSize;
	cellNumY = screenY / cellSize;
	cellNumZ = screenZ / cellSize;

	//Set the size of the cells structure in the x dimension
	cells = new grid**[cellNumX];

	//Set the size of the cells structure in the y dimension
	for (int i = 0; i < cellNumX; i++)
	{
		cells[i] = new grid*[cellNumY];
	}

	//Set the size of the cells structure in the z dimension
	for (int i = 0; i < cellNumX; i++)
	{
		for (int j = 0; j < cellNumY; j++)
		{
			cells[i][j] = new grid[cellNumZ];
		}
	}

	//Set the coordinates of each cell
	//Loop for the number of cells in the X
	for (int i = 0; i < cellNumX; i++)
	{
		//Loop for the number of cells in the Y
		for (int j = 0; j < cellNumY; j++)
		{
			//Loop for the number of cells in the Z
			for (int k = 0; k < cellNumZ; k++)
			{
				cells[i][j][k].minX = i * cellSize;
				cells[i][j][k].maxX = (i + 1) * cellSize;

				cells[i][j][k].minY = j * cellSize;
				cells[i][j][k].maxY = (j + 1) * cellSize;

				cells[i][j][k].minZ = k * cellSize;
				cells[i][j][k].maxZ = (k + 1) * cellSize;

				cells[i][j][k].object = new objects[objCounter];
			}
		}
	}
}

void UniformGrid::setObjectsInGrid()
{
	//Loop for the number of objects
	for (int i = 0; i < objCounter; i++)
	{
		bool objLoaded = false; //Used to store if an object has already been set

		//Loop for the number of cells in the X
		for (int j = 0; j < cellNumX; j++)
		{
			//Loop for the number of cells in the Y
			for (int k = 0; k < cellNumY; k++)
			{
				//Loop for the number of cells in the Z
				for (int l = 0; l < cellNumZ; l++)
				{

					//Check if the object being looked at is in the cell being looked at
					if (object[i].x >= cells[j][k][l].minX && object[i].x <= cells[j][k][l].maxX)
					{
						if (object[i].y >= cells[j][k][l].minY && object[i].y <= cells[j][k][l].maxY)
						{
							if (object[i].z >= cells[j][k][l].minZ && object[i].z <= cells[j][k][l].maxZ)
							{
								//Check if the object has already been added to another cell
								if (!objLoaded)
								{
									//Add the object variables to the cell it's added to
									cells[j][k][l].object[cells[j][k][l].objCount].x = object[i].x;
									cells[j][k][l].object[cells[j][k][l].objCount].y = object[i].y;
									cells[j][k][l].object[cells[j][k][l].objCount].z = object[i].z;
									cells[j][k][l].object[cells[j][k][l].objCount].r = object[i].r;

									objLoaded = true; //Set objLoaded to true

									cells[j][k][l].objCount++; //Increment the number of objects in that cell by 1

									//Check if the cell's objAdded is false
									if (!cells[j][k][l].objAdded)
									{
										cells[j][k][l].objAdded = true; //Set the cell's objAdded to true
										cellObjCount++; //Increment the number of objects added to cells by 1
									}
									
								}
							}
						}
					}
				}
			}
		}
	}

	gCO = new gridsConObj[cellObjCount]; //Set the size of gCO
}

grid*** UniformGrid::getGrid()
{
	return cells; //Return the cells
}

int UniformGrid::getCellNumX()
{
	return cellNumX; //Return the number of cells in the X
}

int UniformGrid::getCellNumY()
{
	return cellNumY; //Return the number of cells in the Y
}

int UniformGrid::getCellNumZ()
{
	return cellNumZ; //Return the number of cells in the Z
}