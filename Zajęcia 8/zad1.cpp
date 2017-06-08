#include "stdafx.h"
#include <GL/glut.h>
#include "gltools_extracted.h"
#include "VectorMath.cpp"
#define GL_PI 3.14158265358

void createObject()
{
	GLTVector3 centralVertex{ 0.0f, -1.0f, 0.0f };
	GLTVector3 topVertex{ 0.0f, 2.0f, 0.0f };
	GLTVector3 bottomVertices[8];

	for (size_t i = 0; i < 8; i++)
	{
		GLfloat angle = 2.0 * GL_PI * i / 8.0f;
		GLfloat x = 2 * sin(angle);
		GLfloat y = 2 * cos(angle);

		bottomVertices[i][0] = x;
		bottomVertices[i][1] = -1.0f;
		bottomVertices[i][2] = y;
	}

	glBegin(GL_POLYGON);
	//Podstawa
	for (size_t i = 0; i < 8; i++)
	{
		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(centralVertex);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(bottomVertices[(i + 1) % 8]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(bottomVertices[i]);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	//Sciany boczne
	for (size_t i = 0; i < 8; i++)
	{
		GLTVector3 normal;
		gltGetNormalVector(bottomVertices[i], bottomVertices[(i + 1) % 8], topVertex, normal);
		glNormal3fv(normal);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(bottomVertices[i]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(bottomVertices[(i + 1) % 8]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(topVertex);
	}

	glEnd();
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// w³¹czenie bufora szablonowego
	glEnable(GL_STENCIL_TEST);


	// rysujemy szeœcian i uzupe³niamy bufor szablonowy
	// jedynkami w tym miejscu gdzie narysujemy szeœcian

	glStencilFunc(GL_ALWAYS, 1, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	createObject();

	// uzupe³niamy bufor szablonowy dwójkami w tych miejscach
	// gdzie kula pokrywa szeœcian
	glStencilFunc(GL_ALWAYS, 2, 0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glutSolidCube(2.2);

	// // czyszczenie bufora koloru i bufora g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wy³¹czenie testu bufora szablonowego i rysejemy czerwon¹ kulê
	glDisable(GL_STENCIL_TEST);
	glColor3d(1, 0, 0);
	glutSolidSphere(0.7, 100, 100);

	// zielony torus obracany na 80 stopni wed³ug niebieskiego
	glColor3d(0, 0, 1);
	glPushMatrix();
	glRotated(80, 1, 0, 0);
	glutSolidTorus(0.15, 0.8, 80, 100);
	glPopMatrix();

	// jeszcze raz pozwalamy test buforu szablonowego
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 255);
	glColor3d(1, 1, 1);
	createObject();

	// wracamy scenê

	glRotated(0.3, 1, 0, 0);
	glRotated(0.5, 0, 1, 0);
	glRotated(0.7, 0, 0, 1);

	glFlush();

	glutSwapBuffers();
}


void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5, 5, -5, 5, 2, 12);
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	display();
}

///////////////////////////////////////////////////////////
// Wywo³ywana przez bibliotek GLUT w czasie, gdy okno nie
// jest przesuwane ani nie jest zmieniana jego wielkoœæ
void TimerFunction(int value) {

	display();
	// Ponowne rysowanie sceny z nowymi wspó³rzêdnymi  
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	float pos[4] = { 3,3,3,1 };
	float dir[3] = { -1,-1,-1 };

	GLfloat mat_specular[] = { 1,1,1,1 };

	glutInitWindowPosition(50, 10);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Stencil buffer");
	glutInitDisplayMode(GL_DOUBLE | GL_RGB | GL_DEPTH);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glEnable(GL_DEPTH_TEST);


	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

	/*
	* Enter your code here
	*/
	glutTimerFunc(33, TimerFunction, 1);
	glutMainLoop();
	return 0;
}