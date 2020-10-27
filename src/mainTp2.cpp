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

	gluLookAt(-25.0, 10.0, 0.0, 0, 5, 0, 0.0, 1.0, 0.0);

	// Draw ground
	glColor3f(0.1f, 0.9f, 0.1f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Render each particle in turn
	for (unsigned i = 0; i < NB_PARTICLES; i++) {
		Vector3 position;
		position = pw.particles[i].getPosition();

		glColor3f(0, 0, 0);
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(PARTICLE_RADIUS, 10, 10);
		glPopMatrix();
	}

	std::cout << pw.particles[0].getPosition().y << std::endl;

	glutSwapBuffers();

	glClearColor(0.9f, 0.95f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void special(int key, int xx, int yy) {}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 27: // Code ASCII de échap
		exit(EXIT_SUCCESS);
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
