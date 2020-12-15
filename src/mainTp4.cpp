#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time temps = Time::getInstance();

std::vector<std::vector<World::Object>> potentialContact;

enum TypeCam
{
	box,
	global,
};

TypeCam typeCam = box;

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
	
	//Box  -----------------------------------
	Matrix4 transform2 = world.myBox2.colBox.body.getTransform();
	GLfloat glutMatrix2[16];
	transform2.fillGLArray(glutMatrix2);
	glPushMatrix();
	glMultMatrixf(glutMatrix2);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
	//Box -----------------------------------

}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void update()
{
	temps.update();

	float duration = (float)temps.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	//------------------------------------

	potentialContact = world.CheckPotentialCollision();

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
		gluLookAt(-50, 30, 0, 0, 0, 0, 0.0, 1.0, 0.0);
		break;
	}

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

	case 'r': //Reset
	{
		
	}
	break;

	case 't':
	{
		ForceGenerator* goRight = new Gravity(Vector3(0,0,1) * 6);
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
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return EXIT_SUCCESS;
}