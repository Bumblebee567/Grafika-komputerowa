#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "glext.h"

enum
{
	MAG_FILTER,     // filtr powiêkszaj¹cy
	MIN_FILTER,     // filtr pomniejszaj¹cy
	FULL_WINDOW,    // aspekt obrazu - ca³e okno
	ASPECT_1_1,     // aspekt obrazu 1:1
	EXIT            // wyjœcie
};

int aspect = FULL_WINDOW;
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;
GLfloat scale = 1.05;
GLint RECT_LIST;
GLint TEXTURE_256_LIST, TEXTURE_128_LIST, TEXTURE_64_LIST;
GLint mag_filter = GL_NEAREST;
GLint min_filter = GL_NEAREST;

void DisplayScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -(near + far) / 2);
	glScalef(scale, 1.0, 1.0);
	glEnable(GL_TEXTURE_1D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, min_filter);
	if (mag_filter == GL_LINEAR)
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glCallList(TEXTURE_256_LIST);
	glCallList(TEXTURE_128_LIST);
	glCallList(TEXTURE_64_LIST);
	glFlush();
	glutSwapBuffers();
}
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (aspect == ASPECT_1_1)
	{
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else

			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);
	DisplayScene();
}
void Keyboard(unsigned char key, int x, int y)
{
	if (key == '+')
		scale += 0.05;
	else
		if (key == '-' && scale > 0.05)
			scale -= 0.05;
	DisplayScene();
}

void Menu(int value)
{
	switch (value)
	{
	case MAG_FILTER:
		if (mag_filter == GL_NEAREST)
			mag_filter = GL_LINEAR;
		else
			mag_filter = GL_NEAREST;
		DisplayScene();
		break;

	case MIN_FILTER:
		if (min_filter == GL_NEAREST)
			min_filter = GL_LINEAR;
		else
			min_filter = GL_NEAREST;
		DisplayScene();
		break;

	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
	case EXIT:
		exit(0);
	}
}
void GenerateDisplayLists()
{
	RECT_LIST = glGenLists(1);
	glNewList(RECT_LIST, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord1f(1.0);
	glVertex2f(1.5, 0.7);
	glTexCoord1f(0.0);
	glVertex2f(-1.5, 0.7);
	glTexCoord1f(0.0);
	glVertex2f(-1.5, -0.7);
	glTexCoord1f(1.0);
	glVertex2f(1.5, -0.7);
	glEnd();
	glEndList();
	GLint size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	if (size < 256)
	{
		printf("Rozmiar tekstur mniejszy od 256");
		exit(0);
	}

	GLubyte texture[2048 * 3];
	for (int i = 0; i < 2048; i++)
	{
		texture[3 * i + 0] = i / 8;
		texture[3 * i + 1] = i / 8;
		texture[3 * i + 2] = i / 8;
	}
	TEXTURE_256_LIST = glGenLists(1);
	glNewList(TEXTURE_256_LIST, GL_COMPILE);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	glCallList(RECT_LIST);
	glPopMatrix();
	glEndList();
	for (int i = 0; i < 1024; i++)
	{
		texture[i] = i / 4;
	}
	TEXTURE_128_LIST = glGenLists(1);
	glNewList(TEXTURE_128_LIST, GL_COMPILE);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE, 1024, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, texture);
	glCallList(RECT_LIST);
	glEndList();
	for (int i = 0; i < 512; i++)
	{
		texture[3 * i] = i / 2;
	}
	TEXTURE_64_LIST = glGenLists(1);
	glNewList(TEXTURE_64_LIST, GL_COMPILE);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_INTENSITY, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glPushMatrix();
	glTranslatef(0.0, -2.0, 0.0);
	glCallList(RECT_LIST);
	glPopMatrix();
	glEndList();
}
void ExtensionSetup()
{
	const char *version = (char*)glGetString(GL_VERSION);
	int major = 0, minor = 0;
	if (sscanf_s(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("B³êdny format wersji OpenGL\n");
#else

		printf("Bledny format wersji OpenGL\n");
#endif

		exit(0);
	}
	if (!(major > 1 || minor >= 2) &&
		!glutExtensionSupported("GL_SGIS_texture_edge_clamp"))
	{
		printf("Brak rozszerzenia GL_SGIS_texture_edge_clamp!\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Zadanie 1");
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("Aspekt obrazu - ca³e okno", FULL_WINDOW);
#else
	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	glutCreateMenu(Menu);

#ifdef WIN32

	glutAddMenuEntry("Filtr powiêkszaj¹cy: GL_NEAREST/GL_LINEAR", MAG_FILTER);
	glutAddMenuEntry("Filtr pomniejszaj¹cy: GL_NEAREST/GL_LINEAR", MIN_FILTER);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjœcie", EXIT);
#else

	glutAddMenuEntry("Filtr powiekszajacy: GL_NEAREST/GL_LINEAR", MAG_FILTER);
	glutAddMenuEntry("Filtr pomniejszajacy: GL_NEAREST/GL_LINEAR", MIN_FILTER);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	GenerateDisplayLists();
	ExtensionSetup();
	glutMainLoop();
	return 0;
}