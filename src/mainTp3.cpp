#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time time = Time::getInstance();
Vector3 massCenter = Vector3(0, 10, 10);
int demiLenght = 1;
float lambda = 0.1;

void renderBitmapString(float x, float y, float z, void* font, char* string) {

	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void DrawCube(Vector3 massCenter, float r, float g, float b)
{
	//Lignes du cube
	glBegin(GL_LINES);
	
	glColor3f(r, g, b);

	//Plateforme du haut
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);

	//Colonnes
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y + demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);

	//Plateforme du bas
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x + demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z + demiLenght);
	glVertex3f(massCenter.x - demiLenght, massCenter.y - demiLenght, massCenter.z - demiLenght);

	glEnd();

	//Centre de masse
	glBegin(GL_QUADS);

	glColor3f(1, 0, 0);

	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z - lambda);


	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z - lambda);


	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z + lambda);


	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z + lambda);

	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y + lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y + lambda, massCenter.z + lambda);

	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z - lambda);
	glVertex3f(massCenter.x + lambda, massCenter.y - lambda, massCenter.z + lambda);
	glVertex3f(massCenter.x - lambda, massCenter.y - lambda, massCenter.z + lambda);


	glEnd();
	
	
}

void update() 
{
	world.startFrame();
	time.update();

	float duration = (float)time.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	//HERE

	world.runPhysics(duration);
	glutPostRedisplay();
}


void reshape(int w, int h) 
{
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//-------------------------------------------------

	gluLookAt(massCenter.x - 15, massCenter.y + 10, massCenter.z, massCenter.x, massCenter.y, massCenter.z, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();

	//Draw gizmo
	glColor3f(0, 0, 1);
	glPushMatrix();
	glPopMatrix();

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1,0,0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0,1,0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0,0,1);

	
	DrawCube(massCenter, 1, 1, 1);
	

	//-------------------------------------------------

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void special(int key, int xx, int yy) 
{
	
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//HERE
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	time.init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("TP3");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glutMainLoop();
	return EXIT_SUCCESS;
}