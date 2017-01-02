#include "myDrawing.h"
#include "MyScreen.h"

// MyScreen 참조변수 screen
MyScreen screen;

// 디스플레이 콜백함수
void MyDisplay() {
	screen.display();
}

// 마우스 클릭 콜백함수
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
	screen.mouseClick(Button, State, X, Y);
}

// 마우스 무브 콜백함수
void MyMouseMove(GLint X, GLint Y) {
	screen.mouseMove(X, Y);
}

// 메뉴 콜백 함수
void MyMainMenu(int entryID) {
	screen.mainMenu(entryID);
}

// 타이머 콜백 함수
void MyTimer(int value) {
	screen.timer(value);
	glutTimerFunc(40, MyTimer, 1);
}

// 키보드 콜백 함수
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	screen.keyboard(KeyPressed, X, Y);
}

int main(int argc, char* argv[]) {
	/* 윈도우 초기화 및 생성 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// 더블 버퍼를 사용한다.
	glutInitWindowSize(300, 300);
	glutCreateWindow("My Robot");

	glutDisplayFunc(MyDisplay);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);
	glutKeyboardFunc(MyKeyboard);
	glutTimerFunc(40, MyTimer, 1);   // 40밀리초 이후 실행

	// 메뉴 콜백함수 등록
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	// 현 메뉴에 메뉴 항목을 추가한다.
	screen.addMenuEntry();
    glutMainLoop();
    return 0;
}
