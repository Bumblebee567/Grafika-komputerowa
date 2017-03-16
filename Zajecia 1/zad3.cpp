#include <cmath>
#include <GL/glut.h>
#define GL_PI 3.1415926535f


GLfloat vertices[8][2];
GLfloat rsize = 50.0f, polygonCenterPosition[2]{ 125.0f,125.0f };
GLfloat xstep = 2.0f, ystep = 2.0f;
GLfloat windowWidth, windowHeight;


void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_POLYGON);
	for (size_t i = 0; i < 8; i++)
		glVertex2f(vertices[i][0], vertices[i][1]);

	glEnd();
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glutSwapBuffers();
}

void TimerFunction(int value)
{
	if (polygonCenterPosition[0] > windowWidth - rsize || polygonCenterPosition[0] < rsize)
		xstep = -xstep;

	if (polygonCenterPosition[1] > windowHeight - rsize || polygonCenterPosition[1] < rsize)
		ystep = -ystep;

	if (polygonCenterPosition[0] > windowWidth - rsize)
		polygonCenterPosition[0] = windowWidth - rsize - xstep;

	if (polygonCenterPosition[1] > windowHeight - rsize)
		polygonCenterPosition[1] = windowHeight - rsize - ystep;


	for (size_t i = 0; i < 8; i++)
	{
		vertices[i][0] += xstep;
	}

	for (size_t i = 0; i < 8; i++)
	{
		vertices[i][1] += ystep;
	}

	polygonCenterPosition[0] += xstep;
	polygonCenterPosition[1] += ystep;

	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

void resize(GLsizei w, GLsizei h)
{

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		windowHeight = 250.0f*h / w;

		windowWidth = 250.0f;
	}
	else
	{
		windowWidth = 250.0f*w / h;

		windowHeight = 250.0f;
	}

	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char* argv[])
{

	for (size_t i = 0; i < 8; i++)
	{
		vertices[i][0] = polygonCenterPosition[0] + rsize * std::sinf(2 * GL_PI * ((GLfloat)i / 8));
		vertices[i][1] = polygonCenterPosition[1] + rsize * std::cosf(2 * GL_PI * ((GLfloat)i / 8));
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("Wielokat");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(resize);
	glutTimerFunc(90, TimerFunction, 10);
	glutMainLoop();
}