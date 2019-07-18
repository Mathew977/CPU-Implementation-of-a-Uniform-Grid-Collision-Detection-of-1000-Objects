#include "Stdafx.h"
#include "UniformGrid.h"
#include "ObjectLoader.h"

#define SCREEN_X 100 //Max x size of the grid
#define SCREEN_Y 100 //Max y size of the grid
#define SCREEN_Z 100 //Max z size of the grid

float cameraX = SCREEN_X / 2;
float cameraY = SCREEN_Y / 2;
float cameraZ = SCREEN_Z / 2;
float totalAngleX = 0.0;
float totalAngleY = 0.0;

bool red = true, green = true, gridShown = true; //Used to toggle the red object, the green objects, and the cell on or off

ObjectLoader obj;
UniformGrid grids;

grid*** cell;

int* x;
int* y;
int* z;

void collisionDetection();
bool detectionCheck(int, int, int);
int collisionChecker(int, int, int, int, int, int);
int currentCellCollisionDetection(int, int, int);

//Handles the user inputs
void keyboardread(unsigned char key, int x, int y)
{
	//Values used to alter the camera
	float vx = 0, vy = 0, vz = 0, xAngle = 0, yAngle = 0;

	switch (key)
	{
	case 'w':
	{
		vz--; //Move camera forward
		break;
	}
	case 'd':
	{
		vx++; //Move camera right
		break;
	}
	case 's':
	{
		vz++; //Move camera back
		break;
	}
	case 'a':
	{
		vx--; //Move camera left
		break;
	}
	case 'i':
	{
		yAngle -= 10; //Rotate camera up
		break;
	}
	case 'l':
	{
		xAngle += 10; //Rotate camera right
		break;
	}
	case 'k':
	{
		yAngle += 10; //Rotate camera down
		break;
	}
	case 'j':
	{
		xAngle -= 10; //Rotate camera left
		break;
	}
	case 'g':
	{
		//Toggles the green objects on and off
		if (green)
			green = false;
		else
			green = true;

		break;
	}
	case 'r':
	{
		//Toggles the red objects on and off
		if (red)
			red = false;
		else
			red = true;

		break;
	}
	case 'f':

		//Toggles the grid on and off
		if (gridShown)
			gridShown = false;
		else
			gridShown = true;

		break;
	default:
		break;
	}

	cameraX += vx; //Change the cameraX value, making the camera move in the x
	cameraY += vy; //Change the cameraY value, making the camera move in the y
	cameraZ += vz; //Change the cameraZ value, making the camera move in the z

	totalAngleX += xAngle; //Change the totalAngleX value, making the camera rotate around the y
	totalAngleY += yAngle; //Change the totalAngleY value, making the camera rotate around the z

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

//Handles displaying the objects and grid
void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen and depth buffer
	glLoadIdentity();										// reset modelview matrix

	glRotatef(totalAngleX, 0, 1, 0); //Rotate around the y
	glRotatef(totalAngleY, 1, 0, 0); //Rotate around the x

	gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY, cameraZ - 1, 0.0, 1.0, 0.0); //reset camera position
	glColor3f(0, 0, 0);  //set colour
	glTranslatef(0.5, 0.5, 0.5);  //move by x,y and z units

	int colCount = 0;

	//Check if the grid needs to be shown
	if (gridShown)
	{
		//Loop for the number of cells in the X
		for (int i = 0; i < grids.getCellNumX(); i++)
		{
			//Loop for the number of cells in the Y
			for (int j = 0; j < grids.getCellNumY(); j++)
			{
				//Loop for the number of cells in the Z
				for (int k = 0; k < grids.getCellNumZ(); k++)
				{
					//Draw bottom of cell
					glBegin(GL_LINE_LOOP);
					glColor3f(1.0, 1.0, 0.0);

					glVertex3f(cell[i][j][k].minX, cell[i][j][k].minY, cell[i][j][k].minZ);
					glVertex3f(cell[i][j][k].minX, cell[i][j][k].minY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].minY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].minY, cell[i][j][k].minZ);

					glEnd();

					//Draw top of cell
					glBegin(GL_LINE_LOOP);
					glColor3f(1.0, 1.0, 0.0);

					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].maxY, cell[i][j][k].minZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].maxY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].minX, cell[i][j][k].maxY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].minX, cell[i][j][k].maxY, cell[i][j][k].minZ);

					glEnd();

					//Draw front of cell
					glBegin(GL_LINE_LOOP);
					glColor3f(1.0, 1.0, 0.0);

					glVertex3f(cell[i][j][k].minX, cell[i][j][k].minY, cell[i][j][k].minZ);
					glVertex3f(cell[i][j][k].minX, cell[i][j][k].maxY, cell[i][j][k].minZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].maxY, cell[i][j][k].minZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].minY, cell[i][j][k].minZ);

					glEnd();
					glBegin(GL_LINE_LOOP);
					glColor3f(1.0, 1.0, 0.0);

					glVertex3f(cell[i][j][k].minX, cell[i][j][k].minY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].minX, cell[i][j][k].maxY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].maxY, cell[i][j][k].maxZ);
					glVertex3f(cell[i][j][k].maxX, cell[i][j][k].minY, cell[i][j][k].maxZ);

					glEnd();
				}
			}
		}
	}
	
	//Loop through for the number objects
	for (int i = 0; i < grids.cellObjCount; i++)
	{
		//Loop for the number of objects in the cell being looked at
		for (int j = 0; j < cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].objCount; j++)
		{
			//Check if the object being looked at has collided and if red is toggled on
			if (cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].collided && red)
			{
				glPushMatrix();
				glTranslatef(cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].x, cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].y, cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].z);
				
				glColor3f(1.0, 0.0, 0.0);
				glutSolidSphere(cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].r, 20, 20);
				glPopMatrix();
			}
			//Check if the object being looked at has collided and if green is toggled on
			else if (!cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].collided && green)
			{
				glPushMatrix();
				glTranslatef(cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].x, cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].y, cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].z);
				
				glColor3f(0.0, 1.0, 0.0);
				glutSolidSphere(cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].object[j].r, 20, 20);
				glPopMatrix();
			}
		}
	}

	glFlush();  //force drawing

	glutSwapBuffers();
}

void reshape(int w, int h)
//reshape
//************************************************************************
//OpenGL reshape callback is called when the window is resized
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //respecify size of view port to match new w and h
	glMatrixMode(GL_PROJECTION);  //go into projection mode
	glLoadIdentity();   //reset projection matrix
	//glOrtho(0.0, GRIDSIZE, 0.0, GRIDSIZE, 0.0, 100.0); //defines orthoganal projection (parrallel
													   // viewing volume
													   // window limits of display
													   // left =0.0, right =2.0,
													   // bottom = 0.0, top = 2.0
													   // near =0.0, far = 100.0
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);  //redefine projection matrix to match changes in w and h
	glMatrixMode(GL_MODELVIEW);  //switch back to modelview mode
	glLoadIdentity(); //reset modelview matrix
	gluLookAt(0, 0, 0, cameraX, cameraY, cameraZ, 0.0, 1.0, 0.0); //reset camera position
}

void glutMain(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	//init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardread);
	glutMainLoop();
}

void main(int argc, char* argv[])
{
	int objCount; //Stores the number of objects read from a file

	objCount = obj.objectCounter(); //Counts the number of objects to be loaded in

	grids.setobj(objCount); //Reads in the objects from the file

	float rad; //Stores the radius of the largest object

	rad = obj.objLoader(grids);

	grids.objCounter /= 4; //Divide the object counter by 4 since each object has four variables read in, making the count 4 times larger than it should be

	grids.setGrid(SCREEN_X, SCREEN_Y, SCREEN_Z, rad); //Creates the grid and cells

	grids.setObjectsInGrid(); //Add the objects to the cells they belong to

	x = new int[grids.objCounter]; //Set the size of the x array
	y = new int[grids.objCounter]; //Set the size of the y array
	z = new int[grids.objCounter]; //Set the size of the z array

	cell = grids.getGrid();

	int itemCount = 0;

	int setgCO = 0;

	//Loop for the number of cells in the X
	for (int i = 0; i < grids.getCellNumX(); i++)
	{
		//Loop for the number of cells in the Y
		for (int j = 0; j < grids.getCellNumY(); j++)
		{
			//Loop for the number of cells in the Z
			for (int k = 0; k < grids.getCellNumZ(); k++)
			{
				//Check if the object count for the cell being looked at isn't 0
				if (cell[i][j][k].objCount != 0)
				{
					//Set the corresponding gCO x, y, z values to i, j, and k
					grids.gCO[setgCO].x = i;
					grids.gCO[setgCO].y = j;
					grids.gCO[setgCO].z = k;

					setgCO++; //Increment setgCO by 1

					//Loop for the number of objects in the cell being looked at
					for (int l = 0; l < cell[i][j][k].objCount; l++)
					{
						x[itemCount] = i;
						y[itemCount] = j;
						z[itemCount] = k;

						itemCount++; //Increment itemCount by 1
					}
				}
			}
		}
	}

	//Output initialisation values - DEBUGGING
	cout << "CELL X: " << grids.getCellNumX() << endl;
	cout << "CELL Y: " << grids.getCellNumY() << endl;
	cout << "CELL Z: " << grids.getCellNumZ() << endl;
	cout << "SIZE OF GRID: " << grids.getCellNumX() * grids.getCellNumY() * grids.getCellNumZ() << endl;
	cout << "OBJECTS IN CELLS: " << grids.objCounter << endl;
	cout << "OBJECTS SAVED: " << itemCount << endl;
	cout << "CELLS CONTAINING OBJECTS: " << setgCO << endl;
	cout << "FINISHED INITIALISING" << endl;

	//Call the collisionDetection function that handles all the collision detection calculations
	collisionDetection();

	//Run the glut code to output the objects
	glutMain(argc, argv);

	//Pause
	getchar();
}

//Handles collision detections
void collisionDetection()
{
	int collisions = 0; //Stores the number of collisions detected

	//Loop through for the number objects
	for (int i = 0; i < grids.cellObjCount; i++)
	{

		//Check if the cell hasn't already been checked
		if (!cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].checked)
		{
			//Set the variable that determines if the cell has been checked or not to true
			cell[grids.gCO[i].x][grids.gCO[i].y][grids.gCO[i].z].checked = true;

			//Stores if collision detections are needed
			bool ful = false, fum = false, fur = false, fml = false, fmm = false, fmr = false, fdl = false, fdm = false, fdr = false;
			bool mul = false, mum = false, mur = false, mml = false, mmr = false, mdl = false, mdm = false, mdr = false;
			bool bul = false, bum = false, bur = false, bml = false, bmm = false, bmr = false, bdl = false, bdm = false, bdr = false;

			//Check if collision detection is needed for the 9 cells infront of the current cell
			//Check if the cell being looked at isn't at the very front of the grid
			if (grids.gCO[i].z > 0)
			{
				//Check if the cell being looked at is below the top of the grid
				if (grids.gCO[i].y < grids.getCellNumY() - 1)
				{
					//Check if cell being looked at isn't on the far left of the grid
					if (grids.gCO[i].x > 0)
						ful = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z - 1); //Check if a collision detection is needed

					fum = detectionCheck(grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z - 1); //Check if a collision detection is needed

					//Check if cell being looked at isn't on the far right of the grid
					if (grids.gCO[i].x < grids.getCellNumX() - 1)
						fur = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z - 1); //Check if a collision detection is needed
				}

				//Check if cell being looked at isn't on the far left of the grid
				if (grids.gCO[i].x > 0)
					fml = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z - 1); //Check if a collision detection is needed

				fmm = detectionCheck(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z - 1); //Check if a collision detection is needed
				
				//Check if cell being looked at isn't on the far right of the grid
				if (grids.gCO[i].x < grids.getCellNumX() - 1)
					fmr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z - 1); //Check if a collision detection is needed

				//Check if the cell being looked at is above the bottom of the grid
				if (grids.gCO[i].y > 0)
				{
					//Check if cell being looked at isn't on the far left of the grid
					if (grids.gCO[i].x > 0)
						fdl = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z - 1); //Check if a collision detection is needed

					fdm = detectionCheck(grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z - 1); //Check if a collision detection is needed

					//Check if cell being looked at isn't on the far right of the grid
					if (grids.gCO[i].x < grids.getCellNumX() - 1)
						fdr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z - 1); //Check if a collision detection is needed
				}
			}
			//Check if collision detection is needed for the 9 cells infront of the current cell

			//Check if collision detection is needed for the 8 cells around the current cell
			//Check if the cell being looked at is below the top of the grid
			if (grids.gCO[i].y < grids.getCellNumY() - 1)
			{
				//Check if cell being looked at isn't on the far left of the grid
				if (grids.gCO[i].x > 0)
					mul = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z); //Check if a collision detection is needed

				mum = detectionCheck(grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z); //Check if a collision detection is needed

				//Check if cell being looked at isn't on the far right of the grid
				if (grids.gCO[i].x < grids.getCellNumX() - 1)
					mur = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z); //Check if a collision detection is needed
			}

			//Check if cell being looked at isn't on the far left of the grid
			if (grids.gCO[i].x > 0)
				mml = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z); //Check if a collision detection is needed

			//Check if cell being looked at isn't on the far right of the grid
			if (grids.gCO[i].x < grids.getCellNumX() - 1)
				mmr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z); //Check if a collision detection is needed

			//Check if the cell being looked at is above the bottom of the grid
			if (grids.gCO[i].y > 0)
			{
				//Check if cell being looked at isn't on the far left of the grid
				if (grids.gCO[i].x > 0)
					mdl = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z); //Check if a collision detection is needed

				mdm = detectionCheck(grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z); //Check if a collision detection is needed

				//Check if cell being looked at isn't on the far right of the grid
				if (grids.gCO[i].x < grids.getCellNumX() - 1)
					mdr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z); //Check if a collision detection is needed
			}
			//Check if collision detection is needed for the 8 cells around the current cell

			//Check if collision detection is needed for the 9 cells behind the current cell
			//Check if the cell being looked at isn't at the very back of the grid
			if (grids.gCO[i].z != grids.getCellNumZ() - 1)
			{
				//Check if the cell being looked at is below the top of the grid
				if (grids.gCO[i].y != grids.getCellNumY() - 1)
				{
					//Check if cell being looked at isn't on the far left of the grid
					if (grids.gCO[i].x > 0)
						bul = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z + 1); //Check if a collision detection is needed

					bum = detectionCheck(grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z + 1); //Check if a collision detection is needed

					//Check if cell being looked at isn't on the far right of the grid
					if (grids.gCO[i].x != grids.getCellNumX() - 1)
						bur = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z + 1); //Check if a collision detection is needed
				}

				//Check if cell being looked at isn't on the far left of the grid
				if (grids.gCO[i].x > 0)
					bml = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z + 1); //Check if a collision detection is needed

				bmm = detectionCheck(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z + 1); //Check if a collision detection is needed

				//Check if cell being looked at isn't on the far right of the grid
				if (grids.gCO[i].x < grids.getCellNumX() - 1)
					bmr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z + 1); //Check if a collision detection is needed

				//Check if the cell being looked at is above the bottom of the grid
				if (grids.gCO[i].y > 0)
				{
					//Check if cell being looked at isn't on the far left of the grid
					if (grids.gCO[i].x > 0)
						bdl = detectionCheck(grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z + 1); //Check if a collision detection is needed

					bdm = detectionCheck(grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z + 1); //Check if a collision detection is needed

					//Check if cell being looked at isn't on the far right of the grid
					if (grids.gCO[i].x < grids.getCellNumX() - 1)
						bdr = detectionCheck(grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z + 1); //Check if a collision detection is needed
				}

			}
			//Check if collision detection is needed for the 9 cells behind the current cell

			//Actual Collision Detection
			//Front 9 cells
			if (ful)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z - 1);
			if (fum)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z - 1);
			if (fur)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z - 1);

			if (fml)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z - 1);
			if (fmm)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z - 1);
			if (fmr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z - 1);

			if (fdl)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z - 1);
			if (fdm)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z - 1);
			if (fdr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z - 1);
			//Front 9 cells

			//Middle 8 cells
			if (mul)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z);
			if (mum)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z);
			if (mur)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z);

			if (mml)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z);
			if (mmr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z);

			if (mdl)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z);
			if (mdm)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z);
			if (mdr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z);
			//Middle 8 cells

			//Back 9 cells
			if (bul)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y + 1, grids.gCO[i].z + 1);
			if (bum)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y + 1, grids.gCO[i].z + 1);
			if (bur)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y + 1, grids.gCO[i].z + 1);

			if (bml)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y, grids.gCO[i].z + 1);
			if (bmm)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z + 1);
			if (bmr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y, grids.gCO[i].z + 1);

			if (bdl)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x - 1, grids.gCO[i].y - 1, grids.gCO[i].z + 1);
			if (bdm)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x, grids.gCO[i].y - 1, grids.gCO[i].z + 1);
			if (bdr)
				collisions += collisionChecker(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z, grids.gCO[i].x + 1, grids.gCO[i].y - 1, grids.gCO[i].z + 1);
			//Back 9 cells

			 //Call the currentCellCollisionDetection function, which returns the number of collisions found in that checked cell and adds them to the number of collisions
			collisions += currentCellCollisionDetection(grids.gCO[i].x, grids.gCO[i].y, grids.gCO[i].z);
		}
	}

	cout << "COLLISIONS: " << collisions << endl; //Output the number of collisions found
}

//Handles checking if any adjacent cells to the cell being looked at contain objects and need to be checked
bool detectionCheck(int x, int y, int z)
{
	//Check if the adjecent cell being checked has objects in it and hasn't been checked yet
	if (cell[x][y][z].objCount > 0 && !cell[x][y][z].checked)
		return true; //Return true
	else
		return false; //Return false
}

//Handles the collision detection between objects in the cell being checked with objects in adjacent cells
int collisionChecker(int x, int y, int z, int x2, int y2, int z2)
{
	int collisionCount = 0; //Temporary count for the number of collisions found in the cell

	float distance; //Used to store the distance between the objects being checked
	float comRadius; //Used to store the combined radius of the two objects and then gets squared

	//Loop for the number of object in the cell
	for (int i = 0; i < cell[x][y][z].objCount; i++)
	{
		//Loop for the number of objects in the adjacent cell that hasn't been checked yet
		for (int j = 0; j < cell[x2][y2][z2].objCount; j++)
		{
			//Calculate the distance between the two objects using pythagoras without the square root to work in square space
			distance = ((cell[x][y][z].object[i].x - cell[x2][y2][z2].object[j].x) * (cell[x][y][z].object[i].x - cell[x2][y2][z2].object[j].x)
				+ (cell[x][y][z].object[i].y - cell[x2][y2][z2].object[j].y) * (cell[x][y][z].object[i].y - cell[x2][y2][z2].object[j].y)
				+ (cell[x][y][z].object[i].z - cell[x2][y2][z2].object[j].z) * (cell[x][y][z].object[i].z - cell[x2][y2][z2].object[j].z));

			//Add the radiuses of the two objects together
			comRadius = cell[x][y][z].object[i].r + cell[x2][y2][z2].object[j].r;

			//Square the radius to work in square space
			comRadius *= comRadius;

			//Check if distance is less than or equal to the calculated radius - means a collision has occured
			if (distance <= comRadius)
			{
				collisionCount++; //increment the collision counter by 1
				cell[x][y][z].object[i].collided = true; //Set the bool the tells the object it's collided to true
				cell[x2][y2][z2].object[j].collided = true; //Set the bool the tells the object it's collided to true
			}
		}
	}

	return collisionCount; //Return the collision count found in this cell
}

//Handles collision detections between objects in the same cell
int currentCellCollisionDetection(int x, int y, int z)
{
	int collisionCount = 0; //Temporary count for the number of collisions found in the cell

	float distance; //Used to store the distance between the objects being checked
	float comRadius; //Used to store the combined radius of the two objects and then gets squared

	//Check if the cell being checked has more than 1 object in it to determine if collision detections are needed
	if (cell[x][y][z].objCount > 1)
	{
		//Loop for the number of object in the cell
		for (int i = 0; i < cell[x][y][z].objCount; i++)
		{
			//Loop for the number of objects in the cell that haven't been checked yet
			for (int j = i + 1; j < cell[x][y][z].objCount; j++)
			{
				//Calculate the distance between the two objects using pythagoras without the square root to work in square space
				distance = (cell[x][y][z].object[i].x - cell[x][y][z].object[j].x) * (cell[x][y][z].object[i].x - cell[x][y][z].object[j].x)
					+ (cell[x][y][z].object[i].y - cell[x][y][z].object[j].y) * (cell[x][y][z].object[i].x - cell[x][y][z].object[j].x)
					+ (cell[x][y][z].object[i].z - cell[x][y][z].object[j].z) * (cell[x][y][z].object[i].z - cell[x][y][z].object[j].z);

				//Add the radiuses of the two objects together
				comRadius = cell[x][y][z].object[i].r + cell[x][y][z].object[j].r;

				//Square the radius to work in square space
				comRadius *= comRadius;

				//Check if distance is less than or equal to the calculated radius - means a collision has occured
				if (distance <= comRadius)
				{
					collisionCount++; //increment the collision counter by 1
					cell[x][y][z].object[i].collided = true; //Set the bool the tells the object it's collided to true
					cell[x][y][z].object[j].collided = true; //Set the bool the tells the object it's collided to true
				}
			}
		}
	}

	return collisionCount; //Return the collision count found in this cell
}