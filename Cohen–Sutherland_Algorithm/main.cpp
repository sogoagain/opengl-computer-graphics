#include <stdio.h>
#include "CohenSutherland.h"

// 초기 윈도우의 가로 세로 길이
#define INIT_WIDTH 300
#define INIT_HEIGHT 300

// 현재 윈도우의 가로 세로 길이
int window_width = INIT_WIDTH;
int window_height = INIT_HEIGHT;

// p1.x, p1.y -> 처음 마우스를 눌렀을 때의 gl 좌표값
// p2.x, p2.y -> 마우스를 누르고 움직였을 때의 gl 좌표값
Point p1, p2;
// 그려질 영역의 좌하단, 우상단 좌표
Point win1 = {-0.5, -0.5}, win2 = {0.5, 0.5};

// 선분의 클리핑 여부
bool isClipping = false;

// 디스플레이 콜백함수
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	// 그려질 영역 (빨간색)
	glBegin(GL_LINE_LOOP); {
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(win1.x, win1.x, 0);
		glVertex3f(win1.x, win2.x, 0);
		glVertex3f(win2.x, win2.x, 0);
		glVertex3f(win2.x, win1.x, 0);
	} glEnd();

	glColor3f(1.0, 1.0, 1.0);

	// 선 (흰색)
	if(isClipping)
		// 키보드 c가 눌려 클리핑이 되어야 한다면 clipLine으로 선을 그린다.
		clipLine(win1, win2, p1, p2);
	else {
		// 클리핑 되기 전에는 GL_LINE_LOOP으로 입력받은 좌표를 그린다.
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p2.x, p2.y, 0);
		glEnd();
	}
	glFlush();
}

// 마우스 클릭 콜백함수
void MyMouseClick(GLint Button, GLint State, GLint Win_X, GLint Win_Y) {
	
	// 현재 클리핑 된 선분을 그리고 있다면 클리핑 설정을 해제한다.
	if(isClipping)
		isClipping = !isClipping;

	// 마우스 왼쪽 버튼이 눌렸을 때 p1.x, p1.y gl 좌표값을 설정한다.
	if(State ==GLUT_DOWN && Button ==GLUT_LEFT_BUTTON) {
		// 마우스 클릭으로 들어오는 Win_x, Win_Y 좌표는 윈도우 좌표계(glut)다.
		// 윈도우 좌표계를 gl 좌표계로 변경하여야 도형을 그릴 수 있다.
		p1.x = (double)(Win_X - (window_width/2.)) / (double) (window_width/2.);
		p1.y = (double)((window_height/2.) - Win_Y) / (double) (window_height/2.);

		// 시작 좌표를 콘솔에 출력한다.
		printf("시작: (%5.2f,%5.2f)\n", p1.x, p1.y);
	}
}

// 마우스를 클릭한 상태로 움직였을 때의 콜백 함수
void MyMouseMove(GLint Win_X, GLint Win_Y) {
	// 마우스 왼쪽 버튼이 눌린상태로 움직였을 때마다
	// 윈도우 좌표값을 gl좌표 값으로 변경해 p2.x, p2.y에 저장한다.
	p2.x = (double)(Win_X - (window_width/2.)) / (double) (window_width/2.);
	p2.y = (double)((window_height/2.) - Win_Y) / (double) (window_height/2.);

	// 종료 좌표를 콘솔에 출력한다.
	printf("종료: (%5.2f,%5.2f)\n", p2.x, p2.y);

	// 도형을 그려야하기 때문에
	// display콜백함수를 호출한다.
	glutPostRedisplay();
}

// Reshape 콜백함수
void MyReshape(int New_Width, int New_Height) {
	// 윈도우 크기가 바뀌면 그 값을 저장한뒤
	// 뷰포트 크기를 윈도우 크기와 같도록 설정한다.
	window_width = New_Width;
	window_height = New_Height;
	glViewport(0, 0, window_width, window_height);
}

// Keyboard 콜백함수
void MyKeyboard(unsigned char uPressedKey, int x, int y) {
	// 'c'나 'C'가 눌리면 선분을 클리핑한다.
	// isClippinf을 true로 설정한다.
	if(uPressedKey == 'c' || uPressedKey == 'C') {
		isClipping = true;
		glutPostRedisplay();
	}
}

int main(int argc, char* argv[]) {
	/* 윈도우 초기화 및 생성 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
	glutCreateWindow("선분 클리핑");

	/* 콜백함수 등록 */
	glutDisplayFunc(MyDisplay);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(MyKeyboard);

	glutMainLoop();
	return 0;
}