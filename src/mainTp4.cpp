#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time temps = Time::getInstance();

float theta = 0.0f;
float phi = 15.0f;
int last_x;
int last_y;


enum TypeCam
{
	box,
	global,
};

TypeCam typeCam = global;

void Draw()
{
	//Box  -----------------------------------
	Matrix4 transform = world.myBox.colBox.body.getTransform();
	GLfloat glutMatrix[16];
	transform.fillGLArray(glutMatrix);
	glPushMatrix();
	glMultMatrixf(glutMatrix);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
	//Box -----------------------------------

	//Walls---------------------------------

	glColor3f(1, 1, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(-world.wallSize, world.wallSize, -world.wallSize);
	glVertex3f(-world.wallSize, world.wallSize, world.wallSize);
	glVertex3f(-world.wallSize, -world.wallSize, world.wallSize);
	glVertex3f(-world.wallSize, -world.wallSize, -world.wallSize);
	glEnd();

	glColor3f(1, 1, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(world.wallSize, world.wallSize, -world.wallSize);
	glVertex3f(world.wallSize, world.wallSize, world.wallSize);
	glVertex3f(world.wallSize, -world.wallSize, world.wallSize);
	glVertex3f(world.wallSize, -world.wallSize, -world.wallSize);
	glEnd();

	glColor3f(1, 1, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(-world.wallSize, world.wallSize, -world.wallSize);
	glVertex3f(world.wallSize, world.wallSize, -world.wallSize);
	glVertex3f(world.wallSize, -world.wallSize, -world.wallSize);
	glVertex3f(-world.wallSize, -world.wallSize, -world.wallSize);
	glEnd();

	glColor3f(1, 1, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(-world.wallSize, world.wallSize, world.wallSize);
	glVertex3f(world.wallSize, world.wallSize, world.wallSize);
	glVertex3f(world.wallSize, -world.wallSize, world.wallSize);
	glVertex3f(-world.wallSize, -world.wallSize, world.wallSize);
	glEnd();

	//Walls ---------------------------------
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void renderText(float x, float y, const char* text, void* font)
{
	glDisable(GL_DEPTH_TEST);

	// Temporarily set up the view in orthographic projection.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (double)1280, 0.0, (double)720, -1.0, 1.0);

	// Move to modelview mode.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Ensure we have a font
	if (font == NULL) {
		font = GLUT_BITMAP_HELVETICA_10;
	}

	// Loop through characters displaying them.
	size_t len = strlen(text);

	glRasterPos2f(x, y);
	for (const char* letter = text; letter < text + len; letter++) {

		// If we meet a newline, then move down by the line-height
		// TODO: Make the line-height a function of the font
		if (*letter == '\n') {
			y -= 12.0f;
			glRasterPos2f(x, y);
		}
		glutBitmapCharacter(font, *letter);
	}

	// Pop the matrices to return to how we were before.
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
}


void update()
{
	temps.update();

	float duration = (float)temps.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	//------------------------------------

	if (!world.outOfSim)
	{
		world.potentialContact = world.CheckPotentialCollision();

		world.generateContacts();
	}

	//------------------------------------
	world.Update(duration);
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

	switch (typeCam)
	{
	case box:
		gluLookAt(world.myBox.colBox.body.getPosition().x - 20, world.myBox.colBox.body.getPosition().y + 10, world.myBox.colBox.body.getPosition().z,
			world.myBox.colBox.body.getPosition().x, world.myBox.colBox.body.getPosition().y, world.myBox.colBox.body.getPosition().z,
			0.0, 1.0, 0.0);
		break;

	case global:
		gluLookAt(50, 30, 0, 0, 0, 0, 0.0, 1.0, 0.0);
		break;
	}

	glRotatef(-phi, 0, 0, 1);
	glRotatef(theta, 0, 1, 0);

	// Draw ground
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();

	//Draw Box
	glColor3f(0, 0, 0);
	Draw();

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
	{
		exit(EXIT_SUCCESS);
	}
	break;

	case 'r':
	{
		world.myBox.reset();
		world.outOfSim = false;
	}
	break;

	case 's':
	{
		//ForceGenerator* goFront = new Gravity(Vector3(1,0,0) * 6);
		//world.myBox.registry.add(&world.myBox.colBox.body, goFront);
		
		ForceGenerator* goFront = new Oui(Vector3(1, 0, 0) * 6, Vector3(RandomFloat(0.9,-0.9), RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9))*3);
		world.myBox.registry.add(&world.myBox.colBox.body, goFront);
	}
	break;

	case 'q':
	{
		ForceGenerator* goLeft = new Oui(Vector3(0,0,1) * 6, Vector3(RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9)) * 3);
		world.myBox.registry.add(&world.myBox.colBox.body, goLeft);
	}
	break;

	case 'z':
	{
		ForceGenerator* goBack = new Oui(Vector3(-1,0,0) * 6, Vector3(RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9)) * 3);
		world.myBox.registry.add(&world.myBox.colBox.body, goBack);
	}
	break;

	case 'd':
	{
		ForceGenerator* goRight = new Oui(Vector3(0,0,-1) * 6, Vector3(RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9)) * 3);
		world.myBox.registry.add(&world.myBox.colBox.body, goRight);
	}
	break;

	case '&':
		typeCam = box;

		break;

	case '"':
		typeCam = global;

		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	last_x = x;
	last_y = y;
}

void mouseDrag(int x, int y)
{
	theta += (x - last_x) * 0.25f;
	phi += (y - last_y) * 0.25f;

	if (phi < -20.0f) phi = -20.0f;
	else if (phi > 80.0f) phi = 80.0f;

	last_x = x;
	last_y = y;
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	temps.init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("TP4");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseDrag);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return EXIT_SUCCESS;
}