#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time time = Time::getInstance();
Vector3 massCenter1 = world.myCar.body.getPosition(); 

void DrawCar()
{
	Matrix4 transform = world.myCar.body.getTransform();
	GLfloat gl_transform[16];
	transform.fillGLArray(gl_transform);
	glPushMatrix();
	glMultMatrixf(gl_transform);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
	
	Matrix4 transform2 = world.myCar2.body.getTransform();
	GLfloat gl_transform2[16];
	transform2.fillGLArray(gl_transform2);
	glPushMatrix();
	glMultMatrixf(gl_transform2);

	//Dessine les arretes du cube
	glColor3f(0, 0, 0);
	glPushMatrix();
	glutWireCube(1);

	//Dessine le centre de masse
	glColor3f(1, 0, 0);
	glutSolidSphere(0.1,10,10);
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

	world.myCar2.body.clearAccumulators();

	world.myCar2.registry.updateForces(duration);

	world.myCar2.body.integrate(duration);

	std::cout << world.myCar2.body.getPosition().z << std::endl;

	//--------------------------------------------------------

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

	massCenter1 = world.myCar.body.getPosition();

	gluLookAt(massCenter1.x - 20, massCenter1.y + 10, massCenter1.z+5, massCenter1.x, massCenter1.y, massCenter1.z+5, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();

	glColor3f(0, 0, 0);
	DrawCar();

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
		break;

	case 'z':
	{
		ForceGenerator* rotationCCW = new RotationCCW(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() - Vector3(0, 1, 1));
		world.myCar.registry.add(&world.myCar.body, rotationCCW);
		
	}
		break;

	case 's':
	{
	ForceGenerator* rotationCW = new RotationCW(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() - Vector3(0, 1, 1));
	world.myCar.registry.add(&world.myCar.body, rotationCW);
	}
		break;

	case 'n':
	{
		ForceGenerator* goRight = new Gravity(Vector3(0,0,1));
		world.myCar.registry.add(&world.myCar.body, goRight);
		ForceGenerator* goLeft = new Gravity(Vector3(0,0,-1));
		world.myCar2.registry.add(&world.myCar2.body, goLeft);
	}
	break;


	case 't':
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();

		ForceGenerator* carambolage = new Carambolage(world.myCar.body.getInverseInertiaTensor(), world.myCar.body.getPosition() + Vector3(-0.5, 0, 0.5), Vector3(0.5,0,-1));
		world.myCar.registry.add(&world.myCar.body, carambolage);

		ForceGenerator* carambolage2 = new Carambolage(world.myCar2.body.getInverseInertiaTensor(), world.myCar2.body.getPosition() + Vector3(0.5, 0, -0.5), Vector3(-0.5,0,1));
		world.myCar2.registry.add(&world.myCar2.body, carambolage2);
	}
		break;

	case 'm':
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();
	}
		break;

	case 'r':
	{
		world.myCar.reset(0);
		world.myCar2.reset(10);
	}
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