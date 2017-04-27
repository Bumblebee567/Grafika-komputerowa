#include <GL/glut.h>
#include <vector>

// pozycje w menu
enum
{
	RECTANGLE = GL_POLYGON,
	CLEAR_VERTEX,
	EXIT
};

std::vector<GLfloat> vertexX;
std::vector<GLfloat> vertexY;
int primitiveType = GL_POINTS;

void RenderScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);

	// rysowanie prymitywu
	glBegin(primitiveType);
	for (int i = 0; i < vertexX.size(); i++)
		glVertex2i(vertexX[i], vertexY[i]);

	glEnd();

	glColor3f(0.0, 0.0, 0.0);

	// krawêdzie wielok¹ta
	if (primitiveType == GL_POLYGON)
	{
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < vertexX.size(); i++)
			glVertex2i(vertexX[i], vertexY[i]);

		glEnd();
	}

	glPointSize(3.0);

	// rysowanie wierzcho³ków prymitywów
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < vertexX.size(); i++)
		glVertex2i(vertexX[i], vertexY[i]);

	glEnd();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkosci okna
void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	RenderScene();
}

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		vertexX.insert(vertexX.end(), x);
		vertexY.insert(vertexY.end(), glutGet(GLUT_WINDOW_HEIGHT) - y);
		RenderScene();
	}
}

void ContextMenu(int value)
{
	switch (value)
	{
	case GL_POINTS:
		primitiveType = GL_POINTS;
		RenderScene();
		break;
	case GL_LINES:
		primitiveType = GL_LINES;
		RenderScene();
		break;
	case GL_LINE_STRIP:
		primitiveType = GL_LINE_STRIP;
		RenderScene();
		break;
	case GL_POLYGON:
		primitiveType = GL_POLYGON;
		RenderScene();
		break;
	case CLEAR_VERTEX:
		vertexX.erase(vertexX.begin(), vertexX.end());
		vertexY.erase(vertexY.begin(), vertexY.end());
		RenderScene();
		break;
	case EXIT:
		exit(0);
	}
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Wielokat");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(MouseButton);

	int menuPrimitive = glutCreateMenu(ContextMenu);

	glutAddMenuEntry("Punkty (GL_POINTS)", GL_POINTS);
	glutAddMenuEntry("Odcinki (GL_LINES)", GL_LINES);
	glutAddMenuEntry("Lamana (GL_LINE_STRIP)", GL_LINE_STRIP);
	glutAddMenuEntry("Wielokat (GL_POLYGON)", GL_POLYGON);

	glutCreateMenu(ContextMenu);

	glutAddSubMenu("Prymitywy", menuPrimitive);
	glutAddMenuEntry("Usun wierzcholki", CLEAR_VERTEX);
	glutAddMenuEntry("Wyjscie", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}