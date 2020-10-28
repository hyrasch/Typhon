#include <gl/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <typhon/typhon.h>

using namespace typhon;

ParticleWorld pw;

Time time = Time::getInstance();

// angle of rotation for the camera direction
float angle = 3.14;
// actual vector representing the camera's direction
float lx = 0.0f, lz = 1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

void renderBitmapString(float x, float y, float z, void* font, char* string) {

	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void update() {
	pw.startFrame();
	time.update();

	float duration = (float)time.getFrameDuration() * 0.001f;
	if (duration <= 0.0f) return;

	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		ParticleBuoyancy* buoyancy = new ParticleBuoyancy(0.5, 0.03, 1);

		if (pw.particles[i].getFlaque() && pw.particles[i].getIndexBuo() == 0) {

			pw.particles[i].setIndexBuo(pw.registry.add(pw.particles + i, buoyancy));

		}

		else if (!pw.particles[i].getFlaque() && pw.particles[i].getIndexBuo() != 0) {
			pw.registry.remove(pw.particles + i, pw.particles[i].getIndexBuo());
			pw.particles[i].setIndexBuo(0);
		}
	}
	pw.runPhysics(duration);

	glutPostRedisplay();
}


void reshape(int w, int h) {
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(pw.particles[0].getPosition().x - 25.0, pw.particles[0].getPosition().y + 10.0, pw.particles[0].getPosition().z, pw.particles[0].getPosition().x, pw.particles[0].getPosition().y, pw.particles[0].getPosition().z, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0.1f, 0.9f, 0.1f);
	glBegin(GL_QUADS);
	glVertex3f(-50, 0.0f, -50);
	glVertex3f(-50, 0.0f, 50);
	glVertex3f(50, 0.0f, 50);
	glVertex3f(50, 0.0f, -50);
	glEnd();
	// Water
	glColor3f(0.2f, 0.6f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(-10, 0.1f, -50);
	glVertex3f(10, 0.1f, -50);
	glVertex3f(10, 0.1f, -30);
	glVertex3f(-10, 0.1f, -30);
	glEnd();
	// Cascade
	glBegin(GL_QUADS);
	glVertex3f(-10, 0.1f, -50);
	glVertex3f(-10, -100, -50);
	glVertex3f(10, -100, -50);
	glVertex3f(10, 0.1f, -50);
	glEnd();

	// Render each particle in turn
	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		Vector3 position;
		position = pw.particles[i].getPosition();

		if (i == 0)
			glColor3f(1, 0, 0); // RED
		else if (i >= NB_PARTICLES / 2)
			glColor3f(0, 0, 1); // BLUE
		else
			glColor3f(0, 0, 0); // BLACK
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(PARTICLE_RADIUS, 10, 10);
		glPopMatrix();
	}

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void special(int key, int xx, int yy) {
	switch (key) {
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // Code ASCII de échap
		exit(EXIT_SUCCESS);
	case 'z':
		pw.particles[0].setVelocity(pw.particles[0].getVelocity() + Vector3(2, 0, 0));
		break;
	case 'q':
		pw.particles[0].setVelocity(pw.particles[0].getVelocity() + Vector3(0, 0, -2));
		break;
	case 's':
		pw.particles[0].setVelocity(pw.particles[0].getVelocity() + Vector3(-2, 0, 0));
		break;
	case 'd':
		pw.particles[0].setVelocity(pw.particles[0].getVelocity() + Vector3(0, 0, 2));
		break;
	case 32: // SPACE BAR
		pw.particles[0].setVelocity(pw.particles[0].getVelocity() + Vector3::UP * 10);
		break;
	case 'f':
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	time.init();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("TP2");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glutMainLoop();
	return EXIT_SUCCESS;
}
