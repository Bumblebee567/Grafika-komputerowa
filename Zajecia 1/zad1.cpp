#include "stdafx.h"
#include <cmath>
#include <GL/glut.h>

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.60f, 0.40f, 0.12f, 0.0f);
	glFlush();
}


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutInitWindowSize(400, 400);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
}