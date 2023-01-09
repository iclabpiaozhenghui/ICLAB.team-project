#include <glut.h>

static int key = 0;

void glInit(void)
{
	glEnable(GL_DEPTH_TEST);	//���� �׽�Ʈ ���
	glEnable(GL_NORMALIZE);		//����ȭ --> ��?
	glEnable(GL_SMOOTH);		//�� ������ ������ �ε巴�� �����Ͽ� ĥ��������
	glEnable(GL_LIGHTING);		//���� ����, ���� �޴� ������ ���� �κ� ������ ��ȭ

	GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };	//��� ���� ��?!?
	GLfloat position[] = { 400.0, 300.0, -700.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);	//If enabled, have one or more material parameters track the current color.
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//� face�� � mode

	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float clip_value = 1;
	glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//���ù��� ����

}

void MainMenu(int entryID)
{
	key = entryID;
}