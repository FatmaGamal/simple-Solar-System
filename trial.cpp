//Include necessary header files
#include "glut.h"
#include <cstdlib>
#include <cmath>

//Link with necessary libraries
#pragma comment (lib, "glut32.lib")

//Callback function prototypes
void render(void);
void onKeyboard(unsigned char key, int x, int y);
void onSpecialKey(int key, int x, int y); 
void onFrame(void);
//Since there are no rings to make visualization better I tried to get top view of the project to see easier
//Then I made the rings but still kept them
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);

#define ESC_ASCII 27

//state of window, handle to terminate window
int g_Win;		


const float PI = 3.141592653;
float x, y, z;
float g_Theta = 0.0f;
int g_LastTime, xDragStart, yDragStart;
float g_X = 0.0f, g_Y = 0.0f, g_Z = -3.5f;
double camera_angle_h = 0;
double camera_angle_v = 0;
float spd[4] = {400, 50, 200, 500};
float dis[5] = {0.59, 0.79, 0.99, 1.2, 0.1};
float rad[6] = {0.2, 0.1, 0.09, 0.04, 0.07, 0.02};
bool isDragging = false;

//Lighting
/*bool g_EnableAmbient = true;
bool g_EnableDiffuse = true;
bool g_EnableSpecular = true;
bool g_HighAccuracy = true;

//Light Colors
float g_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float g_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float g_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//Material Colors
float g_MaterialAmbientDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };

int g_Shininess = 80;*/

int main(int argc, char **argv)
{
	//initializing glut
	glutInit(&argc, argv);

	//double buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);				//double buffer with depth
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);							double buffer 
	//glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);							single buffer
	
	//creating window
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("solar system");

	//display buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//projection transformation -> mapping 3D to 2D  -> setting of angle of lenses of camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Enabling
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_LIGHTING);	//Enable lighting
	glEnable(GL_SPECULAR);*/
	glEnable(GL_NORMALIZE);	//Enable auto-normalization of normal vectors
	glShadeModel(GL_SMOOTH);

	//Setup light 0 
	/*float light0Ambient[] = { 0.0f, 0.0f, 0.0f };	//No light ambient component
	//We use global ambient instead
	float lightPosition[4] = { 0.0f, 0.0f, -4.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_Specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5f);
	glEnable(GL_LIGHT0);	//Enable light source 0

	//Setup global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	//Setup material
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g_MaterialAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, g_Specular);
	glMateriali(GL_FRONT, GL_SHININESS, g_Shininess);*/

	glPointSize(1.0);

	//registering callbacks function
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialKey);
	glutDisplayFunc(render);
	glutIdleFunc(onFrame);
	glutMouseFunc(mouseButton); 
	glutMotionFunc(mouseMove); 

	//Go full-screen
	//glutFullScreen();

	//main loop
	glutMainLoop();

	return 0;
}

void render(void)
{
	//Clear display buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//clearing previous transformation to have only one image showing
	glLoadIdentity();

	//view transformation -> where the camera/eye looks
	//must be first after load identity function
	gluLookAt(g_X, g_Y, g_Z, g_X, g_Y, 0, 0, 1, 0);				//y-axis is my primary axis in this model
	glRotated(camera_angle_v, 1.0, 0.0, 0.0);
	glRotated(camera_angle_h, 0.0, 1.0, 0.0);


	//*********************************************************
	//Rings
	//*********************************************************
	//drawing rings with the origin as their origin
	glBegin(GL_POINTS);

	glColor3f(1.0f, 1.0f, 1.0f);				//white ring
	for (int ring = 0; ring < 4; ring++)
		for (float angle = 0; angle <= 2 * PI * 100; angle += 0.1)
		{
		x = (dis[ring]) * cos(angle);
		y = 0;
		z = (dis[ring]) * sin(angle);

		glVertex3f(x, y, z);

		}
	glEnd();


	//*********************************************************
	//Sun
	//*********************************************************
	glColor3f(1.0f, 1.0f, 0.0f);				//yellow sun
	//glutWireSphere(0.3, 30, 30);		
	glutSolidSphere(rad[0], 30, 30);


	//*********************************************************
	//4 Planets 
	//*********************************************************
	glPushMatrix();
	glRotatef(g_Theta*spd[2], 0, 1, 0);
	glTranslatef(0, 0, dis[0]);					//rotating around the sun
	glRotatef(g_Theta*spd[0], 0, 1, 0);			//rotating around its own y-axis 
	glColor3f(0.5, 0, 1);						//purple planet 1
	//glutSolidSphere(0.17, 30, 30);
	//glutWireSphere(0.17, 15, 15);
	glutWireSphere(rad[1], 30, 30);
	glPopMatrix();

	glPushMatrix();
	glRotatef(g_Theta*spd[0], 0, 1, 0);
	glTranslatef(0, 0.0, dis[1]);
	glRotatef(g_Theta*spd[1], 0, 1, 0);	
	glColor3f(1.0f, 0.0f, 0.0f);				//red planet 2
	glutWireSphere(rad[2], 30, 30);
	//glutSolidSphere(0.09, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glRotatef(g_Theta*spd[3], 0, 1, 0);
	glTranslatef(0, 0.0, dis[2]); 
	glRotatef(g_Theta*spd[2], 0, 1, 0);
	glColor3f(0.0f, 1.0f, 1.0f);				//cyan planet 3
	glutWireSphere(rad[3], 30, 30);
	//glutSolidSphere(0.02, 30, 30);
	glPopMatrix();
	

	//1 planet with orbiting moon
	glPushMatrix();
	glRotatef(g_Theta*spd[1], 0, 1, 0);
	glTranslatef(0, 0.0, dis[3]);
	glRotatef(20, 0, 1, 0);
	glColor3f(1.0f, 1.0f, 1.0f);				//white planet 4
	glutWireSphere(rad[4], 30, 30);
	//glutSolidSphere(0.07, 30, 30);
	glPopMatrix();


	//*********************************************************
	//Moon 
	//*********************************************************
	glRotatef(g_Theta*spd[1], 0, 1, 0);			
	glTranslatef(0, 0.0, dis[3]);
	glRotatef(g_Theta*spd[2], 0, 1, 0);
	glTranslatef(0, 0.0, dis[4]);
	glRotatef(g_Theta*spd[3], 0, 1, 0);
	glColor3f(0.0f, 0.0f, 1.0f);				//blue moon
	glutWireSphere(rad[5], 30, 30);
	//glutSolidSphere(0.01, 20, 20);
	glPopMatrix();

	//Output color buffer to screen
	glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'w':
		g_Y += 0.05f;				//moving the camera so the model will move in the opposite direction
		break;

	case 's':
		g_Y -= 0.05f;
		break;

	case 'a':
		g_X += 0.05f;
		break;

	case 'd':
		g_X -= 0.05f;
		break;

	case ESC_ASCII:
		glutDestroyWindow(g_Win);
		exit(0);
	}
}

void onSpecialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_Z += 0.05f;			//increasing the distance between camera and object
		break;

	case GLUT_KEY_DOWN:
		g_Z -= 0.05f;
		break;
	}
	glutPostRedisplay();
}

//changing only the angle which will be rotated by the rotatef function in the next round of the render fundtion
void onFrame()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	g_Theta += 0.0005f * (time - g_LastTime);
	//g_Theta += 0.0005f;
	g_LastTime = time;
	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	if (isDragging) { // only when dragging
		// update the change in angle
		camera_angle_v += (y - yDragStart)*0.3;
		camera_angle_h += (x - xDragStart)*0.3;
		xDragStart = x;
		yDragStart = y;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {			// left mouse button pressed
			isDragging = true;					
			xDragStart = x;					// save x where button first pressed
			yDragStart = y;
		}
		else  {								//left button released
			isDragging = false;
		}
	}
}