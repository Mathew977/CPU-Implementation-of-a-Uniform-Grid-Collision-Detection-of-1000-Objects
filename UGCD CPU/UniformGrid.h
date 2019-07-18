#pragma once

#include "Stdafx.h"

//Stores each object's information
struct objects
{
	float x; //x position of the object
	float y; //y position of the object
	float z; //z position of the object
	float r; //radius of the object

	bool collided = false;
};

//Stores each cell's information in the grid
struct grid
{
	float minX, maxX; //Size of the cell in the x
	float minY, maxY; //Size of the cell in the y
	float minZ, maxZ; //Size of the cell in the z

	int objCount = 0;

	bool checked = false;

	bool objAdded = false;

	objects* object; //Stores the information of the object in the cell
};

struct gridsConObj
{
	int x;
	int y;
	int z;
};

class UniformGrid
{
private:


	grid*** cells;

	int cellNumX;
	int cellNumY;
	int cellNumZ;

public:
	gridsConObj* gCO;

	objects* object;
	int objCounter;
	int cellObjCount;

	float maxR;

	//Handles setting the size of the structure
	void setobj(int);

	//Handles setting the objects values
	float setObject(float, float, float, float, int);

	//Handles creating the grid
	void setGrid(int, int, int, float);

	void setObjectsInGrid();

	grid*** getGrid(void);

	int getCellNumX();
	int getCellNumY();
	int getCellNumZ();

	UniformGrid();
	~UniformGrid();
};

