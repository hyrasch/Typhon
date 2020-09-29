#include <GL/glut.h>
#include <iostream>
#include <cyclone/cyclone.h>

Time time = Time::getInstance();

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

struct Bullet {
	cyclone::Particle particle;
	int type;
	unsigned time;

	void render()
	{
		cyclone::Vector3 position;
		position = particle.getPosition();

		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.3f, 5, 4);
		glPopMatrix();
	}
};


const static unsigned ammoRounds = 16;

Bullet ammo[ammoRounds];

void func()
{
	for (Bullet* shot = ammo; shot < ammo + ammoRounds; shot++)
	{
		shot->type = 0;
	}
}

int holdedWeapon = 1;

void fire()
{

	std::cout << "BAANG!" << std::endl;

	// Find the first available round.
	Bullet* shot;
	for (shot = ammo; shot < ammo + ammoRounds; shot++)
	{
		if (shot->type == 0) break;
	}

	// If we didn't find a round, then exit - we can't fire.
	if (shot >= ammo + ammoRounds) return;

	// Set the properties of the particle
	switch (holdedWeapon)
	{
	case 1:
		shot->particle.setMass(2.0f); // 2.0kg
		shot->particle.setVelocity(0.0f, 0.0f, 35.0f); // 35m/s
		shot->particle.setAcceleration(0.0f, -1.0f, 0.0f);
		shot->particle.setDamping(1.0f);
		break;

	case 2:
		shot->particle.setMass(200.0f); // 200.0kg
		shot->particle.setVelocity(0.0f, 30.0f, 40.0f); // 50m/s
		shot->particle.setAcceleration(0.0f, -20.0f, 0.0f);
		shot->particle.setDamping(1.0f);
		break;

	case 3:
		// Note that this is the kind of laser bolt seen in films,
		// not a realistic laser beam!
		shot->particle.setMass(0.1f); // 0.1kg - almost no weight
		shot->particle.setVelocity(0.0f, 0.0f, 100.0f); // 100m/s
		shot->particle.setAcceleration(0.0f, 0.0f, 0.0f); // No gravity
		shot->particle.setDamping(1.0f);
		break;

	case 4:
		shot->particle.setMass(1.0f); // 1.0kg - mostly blast damage
		shot->particle.setVelocity(0.0f, 0.0f, 10.0f); // 5m/s
		shot->particle.setAcceleration(0.0f, 0.6f, 0.0f); // Floats up
		shot->particle.setDamping(1.0f);
		break;


	}

	// Set the data common to all particle types
	shot->particle.setPosition(x, 1.5f, z);
	shot->time = time.getNow();
	shot->type = holdedWeapon;

	// Clear the force accumulators
	shot->particle.clearAccumulator();
}

void update() {


	time.update();

	// Find the duration of the last frame in seconds
	float duration = (float)time.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	// Update the physics of each particle in turn
	for (Bullet* shot = ammo; shot < ammo + ammoRounds; shot++)
	{
		if (shot->type != 0)
		{
			// Run the physics
			shot->particle.integrate(duration);

			// Check if the particle is now invalid
			if (shot->particle.getPosition().y < 0.0f ||
				shot->time + 5000 < time.getNow() ||
				shot->particle.getPosition().z > 200.0f)
			{
				// We simply set the shot type to be unused, so the
				// memory it occupies can be reused by another shot.
				shot->type = 0;
			}
		}
	}

	glutPostRedisplay();
}




void reshape(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}



void display()
{

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(-25.0, 8.0, 5.0, 0.0, 5.0, 22.0, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0.1f, 0.9f, 0.1f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Render each particle in turn
	for (Bullet* shot = ammo; shot < ammo + ammoRounds; shot++)
	{
		if (shot->type != 0)
		{
			shot->render();
		}
	}

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

}

void special(int key, int xx, int yy)
{
	float fraction = 0.5f;

	switch (key)
	{
	case GLUT_KEY_F1:
		holdedWeapon = 1;
		std::cout << "Arme choisie : PISTOL" << std::endl;
		break;
	case GLUT_KEY_F2:
		holdedWeapon = 2;
		std::cout << "Arme choisie : ARTILLERY" << std::endl;
		break;
	case GLUT_KEY_F3:
		holdedWeapon = 3;
		std::cout << "Arme choisie : FIREBALL" << std::endl;
		break;
	case GLUT_KEY_F4:
		holdedWeapon = 4;
		std::cout << "Arme choisie : LASER" << std::endl;
		break;
	case GLUT_KEY_LEFT:
		angle -= (2 / fraction) * (0.01f);
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += (2 / fraction) * 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 32: // Code ASCII de la barre espace, pas de raccourci défini il me semble.
		fire();
		break;
	case 27: // Code ASCII de échap
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char** argv)
{
	func();

	// init GLUT and create window
	glutInit(&argc, argv);
	time.init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("TP1");

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	// enter GLUT event processing cycle
	glutMainLoop();

	

	return EXIT_SUCCESS;

}
