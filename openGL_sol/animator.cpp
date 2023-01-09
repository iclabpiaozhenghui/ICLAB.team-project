#include <glut.h>

static int key = 0;

void glInit(void)
{
	glEnable(GL_DEPTH_TEST);	//깊이 테스트 사용
	glEnable(GL_NORMALIZE);		//정규화 --> 뭘?
	glEnable(GL_SMOOTH);		//각 정점의 색상을 부드럽게 연결하여 칠해지도록
	glEnable(GL_LIGHTING);		//지엘 조명, 빛을 받는 각도에 따라 로봇 색상이 변화

	GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };	//어따 쓰는 애?!?
	GLfloat position[] = { 400.0, 300.0, -700.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);	//If enabled, have one or more material parameters track the current color.
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//어떤 face에 어떤 mode

	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float clip_value = 1;
	glOrtho(-clip_value, clip_value, -clip_value, clip_value, -clip_value, clip_value);	//가시범위 설정

}

void MainMenu(int entryID)
{
	key = entryID;
}