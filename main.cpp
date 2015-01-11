#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>		// for cos and sin
#include <vector>

extern "C" {
#include "libs/glee/GLee.h"
#include "libs/glut/glut.h"
#include "libs/jpeg/jpeglib.h"
#include "libs/rtg/RTGlib.h"
}

#define  PI 					3.14159
#define  DOG_TEX_ID 			1
#define  DEVIL_TEX_ID 			2
#define  ZEBRA_TEX_ID 			3
#define  WATER_TEX_ID 			4
#define  ENVMAP_ROOM_TEX_ID 	5
#define  ENVMAP_KTH_TEX_ID 		6
#define  ENVMAP_MIRROR_TEX_ID 	7

#define  TEX_UNIT_1				GL_TEXTURE0_ARB
#define  TEX_UNIT_2				GL_TEXTURE1_ARB

#include "openGL_effects_path.h"
#include "libs/bitmap/Bitmap.h"
#include "textures.cpp"
#include "drawing.cpp"

/*! the object */
Hero* devil = new Hero(OPENGL_EFFECTS_PATH "rtg/brood.rtg");
Hero* dog = new Hero(OPENGL_EFFECTS_PATH "rtg/dog.rtg");

double t = 0, time_step = 0.1;

/*! the position of the camera */
float X = 20.0f, Y = 5.0f, Z = 20.0f;

/*! the direction of the look */
float lx = -1.0f, ly = 0.0f, lz = -1.0f;

/*! the params of the window */
int width = 800, height = 600;

/*! true if the zebra texture is applied */
bool isZebraLoaded = true;

/*!
 * draw a floor
 */
void drawFloor(bool texture) {
	if (texture) {
		load_texture(WATER_TEX_ID, TEX_UNIT_1);
		//		loadTexture("water.bmp");
	}
	// Draw A Quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, 10.0f); // Top Left

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(10.0f, 0.0f, 10.0f); // Top Right

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(10.0f, 0.0f, -10.0f); // Bottom Right

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f); // Bottom Left
	glEnd();

	if (texture)
		unload_texture_unit(TEX_UNIT_1);
}

#include "mirror.cpp"
#include "sphere_mapping.cpp"
#include "multitexturing.cpp"

/*!
 * to redraw the window
 */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	t = t + time_step;

	/* blue background */
	//	glClearColor(0.7f, 0.7f, 1.0f, 0.0f);

	/* depth */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	/* enable colors */
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f };
	GLfloat mat_shininess[] = { 0.1f };
	GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);

	/* lighting */
	glEnable(GL_LIGHTING); //enable lighting

	GLfloat light_pos[4] = { -10, 10, 10, 1 };
	GLfloat specular[] = { .5, .5, .5, 1.0 };
	GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glEnable(GL_LIGHT0); //enable light0

	/* make the mirror */
	devil->scale = .8;
	devil->setPos(0, 0, 0, 65);
	dog->scale = .4;
	double r = 3;
	dog->setPos(r * cos(t), -.02, r * sin(t), -360 * (t + PI / 2) / (2 * PI));
	vector<Hero*> h;
	h.push_back(devil);
	h.push_back(dog);
	mirror(&h);

	/* make the sphere maps */
	devil->setPos(5, 2 + cos(t), -5, 10 * t);
	sphereMap_hero(ENVMAP_ROOM_TEX_ID, TEX_UNIT_1, devil);

	dog->setPos(-5, 0, 5, 60);
	sphereMap_hero(ENVMAP_ROOM_TEX_ID, TEX_UNIT_1, dog);

	sphereMap_sphere(ENVMAP_KTH_TEX_ID, TEX_UNIT_1, 0, 9, 0, 3);

	/* make the multi texturing */
	//	multiTex_sphere(ZEBRA_TEX_ID, false, WATER_TEX_ID, false, 3, 9, 3, 2);

	devil->scale = .4;
	devil->setPos(5, 0, 7, -10 * t);
	multiTex_hero(DEVIL_TEX_ID, false, ENVMAP_KTH_TEX_ID, true, devil);

	dog->scale = .2;
	dog->setPos(7, 0, 5, 10 * t);
	multiTex_hero(DOG_TEX_ID, false, ENVMAP_KTH_TEX_ID, true, dog);

	//	glClearColor(0.7f, 1.0f, 0.7f, 0.0f);

	//	glPushMatrix();
	//	glTranslatef(0, 0, 0);
	//	glutSolidSphere(5, 10, 10);
	//	glPopMatrix();

	glutSwapBuffers();
}

/*!
 * the rouutine to redraw the window
 * @param w width
 * @param h height
 */
void reshape(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	width = w;
	height = h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45, 1.0f * w / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(X, Y, Z, X + lx, Y + ly, Z + lz, 0.0f, 1.0f, 0.0f);

}

/*!
 * refresh the display
 */
void refresh() {
	reshape(width, height);
}

/*!
 * apply a rotation of an angle theta on place on lx0 and lz0
 * @param theta angle in radians
 * @param lx0
 * @param lz0
 */
void applyRotation(double theta, float* lx0, float* lz0) {
	double newLx = cos(theta) * *lx0 - sin(theta) * *lz0;
	double newLz = sin(theta) * *lx0 + cos(theta) * *lz0;
	*lx0 = newLx;
	*lz0 = newLz;
}

/*!
 * rotate the point of view from an angle
 * @param theta the angle in radians
 */
void rotate(double theta) {
	applyRotation(theta, &lx, &lz);
	refresh();
}

/*!
 * advance the point of view from a certain distance
 * @param step the distance
 */
void advance(double step) {
	X += step * lx;
	Y += step * ly;
	Z += step * lz;
	refresh();
}

/*!
 * advance perpediculary to the direction looked
 * @param step the distance to advance
 */
void strafe(double step) {
	float lx2 = lx, lz2 = lz;
	applyRotation(PI / 2.0f, &lx2, &lz2);
	X += step * lx2;
	Z += step * lz2;
	refresh();
}

void idle(void) {
	glutPostRedisplay();
}

/*!
 * the routine for handling key input
 * @param key the input key
 * @param x
 * @param y
 */
void keyboard(unsigned char key, int x, int y) {
	x = x; // just to make them used
	y = y; // just to make them used

	switch (key) {
	case ' ':
		time_step = (time_step == 0 ? 0.1 : 0);
		break;
	case '1':
		strafe(-.5f);
		break;
	case '3':
		strafe(.5f);
		break;
	case '4':
		rotate(-.1f);
		break;
	case '6':
		rotate(.1f);
		break;
	case '8':
		advance(.5f);
		break;
	case '2':
		advance(-.5f);
		break;
	case '+':
		Y += 1;
		refresh();
		break;
	case '-':
		Y -= 1;
		refresh();
		break;
	case 'z':
		isZebraLoaded = !isZebraLoaded;
		break;
	default:
		break;//Don't do anything if any other keys are pressed
	}
}

int main(int argc, char** argv) {
  printf("Keys:\n");
  printf("'8' : camera go forward\n");
  printf("'2' : camera go backward\n");
  printf("'1' : camera strafe left\n");
  printf("'3' : camera strafe right\n");
  printf("'4' : camera rotate left\n");
  printf("'6' : camera rotate right\n");
  printf("'+' : camera go higher\n");
  printf("'-' : camera go lower\n");
  printf("'q' : quit\n");
  printf("\n");

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH);

	glutInitWindowSize(width, height);
  glutCreateWindow("openGL_effects");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);//This tells our program which function to send keyboard events to.

  readTexture(OPENGL_EFFECTS_PATH "textures/texture.bmp", DOG_TEX_ID);
  readTexture(OPENGL_EFFECTS_PATH "textures/broodWarrior.bmp", DEVIL_TEX_ID);
  readTexture(OPENGL_EFFECTS_PATH "textures/zebra.bmp", ZEBRA_TEX_ID);
  readTexture(OPENGL_EFFECTS_PATH "textures/water.bmp", WATER_TEX_ID);
  readTexture(OPENGL_EFFECTS_PATH "textures/env.bmp", ENVMAP_ROOM_TEX_ID);
  readTexture(OPENGL_EFFECTS_PATH "textures/kth_garden.bmp", ENVMAP_KTH_TEX_ID);

	glutMainLoop();

	return EXIT_SUCCESS;
}
