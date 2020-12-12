#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

World world;
Time temps = Time::getInstance();

//Conditions Simulation 1
int sim1Timer = 0;
bool isSim1Running = false;
bool gonnaFall = false;
bool isFalling = false;

//Conditions Simulation 2
int sim2Timer = 0;
bool isSim2Running = false;

//Booléens pour l'ancrage.
bool anchorActivated = false;
bool anchored = false;

enum TypeCam
{
	car1,
	car2,
	global,
};

TypeCam typeCam = global;

void DrawCar()
{
	//Voiture 1 -----------------------------------
	Matrix4 transform = world.myCar.body.getTransform();
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
	//Voiture 1 -----------------------------------

	//Voiture 2 -----------------------------------
	Matrix4 transform2 = world.myCar2.body.getTransform();
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
	//Voiture 2 -----------------------------------

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

	//Simulation 1
#pragma region Simulation 1
	if (isSim1Running)
	{
		if (world.myCar.body.getPosition().y > 9.8 && world.myCar.body.getPosition().y < 10.2 && !isFalling)
		{
			if (world.myCar.body.getVelocity().y > 0)
			{
				world.myCar.registry.registrations.clear();

				isFalling = true;
			}
		}
	}

	if (isFalling)
	{
		world.myCar.registry.registrations.clear();

		ForceGenerator* gravity = new Gravity(Vector3::GRAVITY);
		world.myCar.registry.add(&world.myCar.body, gravity);

		isFalling = false;
	}
#pragma endregion

	//Simulation 2
#pragma region Simulation 2
	if (isSim2Running)
	{
		sim2Timer += 1;

		if (sim2Timer > 100)
		{
			world.myCar.registry.registrations.clear();
			world.myCar2.registry.registrations.clear();

			sim2Timer = 0;
			isSim2Running = false;
		}
	}

	//Simulation d'une collision entre les 2 voitures
	float distCarsZ = abs(world.myCar.body.position.z + 1 - world.myCar2.body.position.z - 1);
	float distCarsX = abs(world.myCar.body.position.x - 1 - world.myCar2.body.position.x + 1);
	float distCarsY = abs(world.myCar.body.position.y - 1 - world.myCar2.body.position.y + 1);

	if (distCarsZ <= 1.5)
	{
		world.myCar.registry.registrations.clear();
		world.myCar2.registry.registrations.clear();

		world.myCar.body.setVelocity(0, 0, -world.myCar.body.getVelocity().z / 1.7f);
		world.myCar.body.setAcceleration(0, 0, -world.myCar.body.getAcceleration().z / 1.7f);

		world.myCar2.body.setVelocity(0, 0, -world.myCar2.body.getVelocity().z / 1.7f);
		world.myCar2.body.setAcceleration(0, 0, -world.myCar2.body.getAcceleration().z / 1.7f);

		ForceGenerator* carambolage = new Carambolage(world.myCar.body.getInverseInertiaTensor(), 0);
		world.myCar.registry.add(&world.myCar.body, carambolage);

		ForceGenerator* carambolage2 = new Carambolage(world.myCar2.body.getInverseInertiaTensor(), 1);
		world.myCar2.registry.add(&world.myCar2.body, carambolage2);

		isSim2Running = true;
	}
#pragma endregion

	//Ancrage des cars si on appuie sur A (désactivable en appuyant à nouveau).
#pragma region AnchoredSpring
	if (anchorActivated)
	{
		//Détection de la distance à partir de laquelle il faut ancrer.
		if (distCarsX > 50 || distCarsZ > 50)
		{
			if (!anchored)
			{
				anchored = true;
				ForceGenerator* springHelp1 = new Spring(Vector3(0, 0, 0), &world.myCar2.body, Vector3(0, 0, 0), 1, 2);
				world.myCar.registry.add(&world.myCar.body, springHelp1);
				ForceGenerator* springHelp2 = new Spring(Vector3(0, 0, 0), &world.myCar.body, Vector3(0, 0, 0), 1, 2);
				world.myCar2.registry.add(&world.myCar2.body, springHelp2);
			}
		}
		else
		{
			if (anchored)
			{
				//Désactivation de l'ancrage si l'on est suffisament rapprochés.
				anchored = false;
				world.myCar.registry.registrations.clear();
				world.myCar2.registry.registrations.clear();
			}
		}
	}
#pragma endregion 

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
	case car1:
		gluLookAt(world.myCar.body.getPosition().x - 20, world.myCar.body.getPosition().y + 10, world.myCar.body.getPosition().z + 5,
			world.myCar.body.getPosition().x, world.myCar.body.getPosition().y, world.myCar.body.getPosition().z + 5,
			0.0, 1.0, 0.0);
		break;
	case car2:
		gluLookAt(world.myCar2.body.getPosition().x - 20, world.myCar2.body.getPosition().y + 10, world.myCar2.body.getPosition().z - 5,
			world.myCar2.body.getPosition().x, world.myCar2.body.getPosition().y, world.myCar2.body.getPosition().z + 5,
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

	//Draw Cars
	glColor3f(0, 0, 0);
	DrawCar();

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

	case 'a':
	{
		anchorActivated = !anchorActivated;
		anchored = false;
	}
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
		Vector3 temp = world.myCar.body.getPosition();
		temp.normalise();

		Vector3 temp2 = world.myCar2.body.getPosition();
		temp2.normalise();

		ForceGenerator* goRight = new Gravity((temp2 - temp) * 6);
		world.myCar.registry.add(&world.myCar.body, goRight);
		ForceGenerator* goLeft = new Gravity((temp - temp2) * 6);
		world.myCar2.registry.add(&world.myCar2.body, goLeft);
	}
	break;

	case 't':
	{
		float impulsion = RandomFloat(1.4, 1.1);

		world.myCar.registry.registrations.clear();
		world.myCar.body.setVelocity(0, -world.myCar.body.getVelocity().y * impulsion, 0);
		world.myCar.body.setAcceleration(0, -world.myCar.body.getVelocity().y * impulsion, 0);

		if (world.myCar.body.getAcceleration().y >= 18.0)
			world.myCar.body.setAcceleration(0, 18, 0);

		if (world.myCar.body.getVelocity().y >= 18.0)
			world.myCar.body.setVelocity(0, 18, 0);

		ForceGenerator* trampoline = new Trampoline(world.myCar.body.getInverseInertiaTensor(), Vector3(RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9), RandomFloat(0.9, -0.9)));
		world.myCar.registry.add(&world.myCar.body, trampoline);

		isSim1Running = true;
	}
	break;

	case 'r':
	{
		world.myCar.reset(-20);
		world.myCar2.reset(20);

		isSim1Running = false;
		isSim2Running = false;
	}
	break;

	case '&':
		typeCam = car1;

		break;

	case 'Ú':
		typeCam = car2;

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
	glutCreateWindow("TP3");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return EXIT_SUCCESS;
}