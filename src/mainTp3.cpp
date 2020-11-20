#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time time = Time::getInstance();
Vector3 massCenter = world.myCar.body.getPosition(); 

void renderBitmapString(float x, float y, float z, void* font, char* string) {

	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void DrawCar(Vector3 massCenter, float r, float g, float b)
{
	float lambda = 0.1;

	//Lignes du cube
	glBegin(GL_LINES);
	
	glColor3f(r, g, b);

	//Plateforme du haut
	glVertex3f(world.myCar.frontUpperLeft.x, world.myCar.frontUpperLeft.y, world.myCar.frontUpperLeft.z);
	glVertex3f(world.myCar.backUpperLeft.x, world.myCar.backUpperLeft.y, world.myCar.backUpperLeft.z);

	glVertex3f(world.myCar.backUpperLeft.x, world.myCar.backUpperLeft.y, world.myCar.backUpperLeft.z);
	glVertex3f(world.myCar.backUpperRight.x, world.myCar.backUpperRight.y, world.myCar.backUpperRight.z);

	glVertex3f(world.myCar.backUpperRight.x, world.myCar.backUpperRight.y, world.myCar.backUpperRight.z);
	glVertex3f(world.myCar.frontUpperRight.x, world.myCar.frontUpperRight.y, world.myCar.frontUpperRight.z);

	glVertex3f(world.myCar.frontUpperRight.x, world.myCar.frontUpperRight.y, world.myCar.frontUpperRight.z);
	glVertex3f(world.myCar.frontUpperLeft.x, world.myCar.frontUpperLeft.y, world.myCar.frontUpperLeft.z);
	
	//Plateforme du bas
	glVertex3f(world.myCar.frontLowerLeft.x, world.myCar.frontLowerLeft.y, world.myCar.frontLowerLeft.z);
	glVertex3f(world.myCar.backLowerLeft.x, world.myCar.backLowerLeft.y, world.myCar.backLowerLeft.z);

	glVertex3f(world.myCar.backLowerLeft.x, world.myCar.backLowerLeft.y, world.myCar.backLowerLeft.z);
	glVertex3f(world.myCar.backLowerRight.x, world.myCar.backLowerRight.y, world.myCar.backLowerRight.z);

	glVertex3f(world.myCar.backLowerRight.x, world.myCar.backLowerRight.y, world.myCar.backLowerRight.z);
	glVertex3f(world.myCar.frontLowerRight.x, world.myCar.frontLowerRight.y, world.myCar.frontLowerRight.z);

	glVertex3f(world.myCar.frontLowerRight.x, world.myCar.frontLowerRight.y, world.myCar.frontLowerRight.z);
	glVertex3f(world.myCar.frontLowerLeft.x, world.myCar.frontLowerLeft.y, world.myCar.frontLowerLeft.z);

	//Plateforme de gauche
	glVertex3f(world.myCar.frontUpperLeft.x, world.myCar.frontUpperLeft.y, world.myCar.frontUpperLeft.z);
	glVertex3f(world.myCar.backUpperLeft.x, world.myCar.backUpperLeft.y, world.myCar.backUpperLeft.z);

	glVertex3f(world.myCar.backUpperLeft.x, world.myCar.backUpperLeft.y, world.myCar.backUpperLeft.z);
	glVertex3f(world.myCar.backLowerLeft.x, world.myCar.backLowerLeft.y, world.myCar.backLowerLeft.z);

	glVertex3f(world.myCar.backLowerLeft.x, world.myCar.backLowerLeft.y, world.myCar.backLowerLeft.z);
	glVertex3f(world.myCar.frontLowerLeft.x, world.myCar.frontLowerLeft.y, world.myCar.frontLowerLeft.z);

	glVertex3f(world.myCar.frontLowerLeft.x, world.myCar.frontLowerLeft.y, world.myCar.frontLowerLeft.z);
	glVertex3f(world.myCar.frontUpperLeft.x, world.myCar.frontUpperLeft.y, world.myCar.frontUpperLeft.z);

	//Plateforme de droite
	glVertex3f(world.myCar.frontUpperRight.x, world.myCar.frontUpperRight.y, world.myCar.frontUpperRight.z);
	glVertex3f(world.myCar.backUpperRight.x, world.myCar.backUpperRight.y, world.myCar.backUpperRight.z);

	glVertex3f(world.myCar.backUpperRight.x, world.myCar.backUpperRight.y, world.myCar.backUpperRight.z);
	glVertex3f(world.myCar.backLowerRight.x, world.myCar.backLowerRight.y, world.myCar.backLowerRight.z);

	glVertex3f(world.myCar.backLowerRight.x, world.myCar.backLowerRight.y, world.myCar.backLowerRight.z);
	glVertex3f(world.myCar.frontLowerRight.x, world.myCar.frontLowerRight.y, world.myCar.frontLowerRight.z);

	glVertex3f(world.myCar.frontLowerRight.x, world.myCar.frontLowerRight.y, world.myCar.frontLowerRight.z);
	glVertex3f(world.myCar.frontUpperRight.x, world.myCar.frontUpperRight.y, world.myCar.frontUpperRight.z);


	glEnd();

	//Centre de masse
	glBegin(GL_QUADS);

	glColor3f(1, 0, 0);

	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);


	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);


	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);


	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);

	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y + lambda, world.myCar.massCenter.z + lambda);

	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z - lambda);
	glVertex3f(world.myCar.massCenter.x + lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);
	glVertex3f(world.myCar.massCenter.x - lambda, world.myCar.massCenter.y - lambda, world.myCar.massCenter.z + lambda);


	glEnd();
}

void DrawTaGrosseMere()
{
	Matrix4 transform = world.myCar.body.getTransform();
	GLfloat gl_transform[16];
	transform.fillGLArray(gl_transform);
	glPushMatrix();
	glMultMatrixf(gl_transform);

	// Fuselage
	glPushMatrix();
	glutWireCube(1);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();

	
	glPopMatrix();

}

void update() 
{
	world.startFrame();
	time.update();

	float duration = (float)time.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	//--------------------------------------------------------

	world.myCar.body.clearAccumulators();

	world.myCar.registry.updateForces(duration);



	world.myCar.body.integrate(duration);

	//--------------------------------------------------------

	world.runPhysics(duration);
	world.myCar.updatePos(world);
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

	massCenter = world.myCar.body.getPosition();

	gluLookAt(massCenter.x - 50, massCenter.y + 10, massCenter.z, massCenter.x, massCenter.y, massCenter.z, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();

	//DrawCar(massCenter, 0, 0, 0);
	glColor3f(0, 0, 0);
	DrawTaGrosseMere();

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void special(int key, int xx, int yy) 
{
	
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // Code ASCII de échap
		exit(EXIT_SUCCESS);
	case 'z':
		ForceGenerator * rotationCW = new RotationCW(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() - Vector3(0, 1, 1));
		world.myCar.registry.add(&world.myCar.body, rotationCW);

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