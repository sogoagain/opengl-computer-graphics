#include <stdio.h>
#include "Bresenham.h"

// 초기 윈도우의 가로 세로 길이
#define INIT_WIDTH 300
#define INIT_HEIGHT 300

// 현재 윈도우의 가로 세로 길이
int window_width = INIT_WIDTH;
int window_height = INIT_HEIGHT;

// p1.x, p1.y -> 처음 마우스를 눌렀을 때의 좌표값 (정수형)
// p2.x, p2.y -> 마우스를 누르고 움직였을 때의 좌표값 (정수형)
Point p1 = {-1, -1}, p2= {-1, -1};

// 디스플레이 콜백함수
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	// 픽셀 영역
	for(int i = 0; i <= 10; i++) {
		glBegin(GL_LINE_LOOP); {
			glVertex3f(i + 0.5, 0, 0);
			glVertex3f(i + 0.5, 10, 0);
		} glEnd();

		glBegin(GL_LINE_LOOP); {
			glVertex3f(0, i + 0.5, 0);
			glVertex3f(10, i + 0.5, 0);
		} glEnd();
	}

	// 점P1과 P2를 잇는 선분 그리기
	drawLine (p1, p2);

	glFlush();
}

// 마우스 클릭 콜백함수
void MyMouseClick(GLint Button, GLint State, GLint Win_X, GLint Win_Y) {
	// 마우스 왼쪽 버튼이 눌렸을 때 p1.x, p1.y의 좌표값을 설정한다.
	if(State ==GLUT_DOWN && Button ==GLUT_LEFT_BUTTON) {
		
		// 현재 가시 부피가 x축 y축 모두 0~10이다.
		// 그러므로 마우스가 클릭된 좌표를 윈도우 크기로 나눠준뒤 10을 곱한다.
		// 단, y축의 경우 마우스 클릭 좌표는 아래로 갈수록 증가하므로 10에서 위에서 계산되어 나온 값을 뺀다.
		// 이렇게 계산된 값을 반올림 하기 위해 0.5를 더한 뒤 int형으로 변환한다.
		p1.x = int(((float(Win_X) / (window_width)) * 10.)+0.5);
		p1.y = 10 - int(((float(Win_Y) / (window_height)) * 10.)+0.5);

		// 시작 좌표를 콘솔에 출력한다.
		printf("시작: (%d,%d)\n", p1.x, p1.y);
	}
}

// 마우스를 클릭한 상태로 움직였을 때의 콜백 함수
void MyMouseMove(GLint Win_X, GLint Win_Y) {
	// 마우스 왼쪽 버튼이 눌린상태로 움직였을 때마다
	// 마우스 좌표값을 현재 가시부피에 맞는 좌표값으로 변경해 p2.x, p2.y에 저장한다.
	p2.x = int(((float(Win_X) / (window_width)) * 10.)+0.5);
	p2.y = 10 - int(((float(Win_Y) / (window_height)) * 10.)+0.5);

	// 종료 좌표를 콘솔에 출력한다.
	printf("종료: (%d,%d)\n", p2.x, p2.y);

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

int main(int argc, char* argv[]) {
	/* 윈도우 초기화 및 생성 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
	glutCreateWindow("선 그리기");

	// 가시부피설정
	glOrtho(0, 10, 0, 10, -1, 1);

	/* 콜백함수 등록 */
	glutDisplayFunc(MyDisplay);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);
	glutReshapeFunc(MyReshape);

	glutMainLoop();
	return 0;
}