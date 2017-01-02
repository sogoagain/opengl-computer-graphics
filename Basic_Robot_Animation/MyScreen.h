#pragma once
#include "myDrawing.h"

class MyScreen
{
private:
	bool	bPressed;	// 마우스 버튼 눌림 여부

	// 현재 애니메이션이 활성화된 상태면 TRUE, 아니면 FALSE
	bool	isAnimationOn;
	bool	armUp;		// 애니메이션시 로봇의 팔 움직임 상태

	int		pX, pY;		// 이전 마우스 커서의 위치
	int		dx, dy;		// 마우스 커서 위치의 변화량
	int		inputMsg;	// 메뉴에서 선택한 항목의 인덱스

	// Delta는 애니메이션시 로봇의 팔 각도를 설정하는데 사용된다.
	GLfloat	Delta;

	Matrix4x4 myMat;				// 현재 모델뷰 행렬을 출력하는데 사용

	// 메뉴에서 선택가능한 모드들
	enum MenuList {
		TRANSLATE,
		ROTATE,
		SCALE,
		ANIMATION_ON,
		ANIMATION_OFF,
		NO_ACTION,
		EXIT_PROGRAM
	};

	// 메뉴에서 이동을 선택했을시 마우스 무브 콜백함수다.
	void mouseEventTrans(void) {
		// 마우스가 x축으로 이동이 있었을 시
		// x축으로 dx/300만큼 로봇을 이동시킨다.
		if (dx != 0)
			glTranslatef((GLfloat)dx / (GLfloat)300, 0.0, 0.0);
		// 마우스가 y축으로 이동이 있었을 시
		// y축으로 dy/300만큼 로봇을 이동시킨다.
		if (dy != 0)
			glTranslatef(0.0, -(GLfloat)dy / (GLfloat)300, 0.0);
	}

	// 메뉴에서 회전을 선택했을시 마우스 무브 콜백함수다.
	void mouseEventRotate(void) {
		// 마우스 위치의 변화량에 따라 각각 x축과 y축으로 돌림
		glRotatef(dx, 0.0, 1.0, 0.0);
		glRotatef(dy, 1.0, 0.0, 0.0);
	}

	// 메뉴에서 크기를 선택했을시 마우스 무브 콜백함수다.
	void mouseEventScale(void) {
		// 마우스 커서가 x축으로 이동했을 시
		// 오른쪽으로 이동했으면 모델을 1.1배 크게하고
		// 왼쪽으로 이동했으면 모델을 0.9배 크게한다.
		if (dx > 0)
			glScalef(1.1, 1.1, 1.1);
		else if (dx < 0)
			glScalef(0.9, 0.9, 0.9);
	}

public:

	MyScreen(void) :bPressed(false) {
		Delta = 0;				// Delta 값은 0으로 초기화한다.
		isAnimationOn = false;	// 실행시는 애니메이션 OFF 상태다.
		inputMsg = NO_ACTION;	// 초기 실행시 마우스 이벤트 발생에 아무런 반응을 하지 않는다.
		armUp = true;			// 로봇 팔이 움직이는 상태다.
	}
	~MyScreen(void) { }

	// 디스플레이 콜백 함수
	void display() {
		// 기존의 컬러 버퍼 내용을 모두 지운다.
		// 만약 이 함수가 없으면 새로운 로봇이 이전 로봇위에 겹쳐서 기록된다.
		glClear(GL_COLOR_BUFFER_BIT);

		// 로봇 그리기
		drawMyRobot(Delta);

		glFlush();

		// glutSwapBuffers( )앞에 있는 모든 함수는 백 버퍼에 적용된다.
		// 즉, 증가된 Delta 값을 사용하여 새로이 그려질 로봇은 백 버퍼에 기록되며
		// 그 동안 현재 화면에 보이는 내용은 현재 프런트 버퍼에 있는 이전 로봇이다.
		// 새로운 로봇의 기록이 완료된 순간 glutSwapBuffers( ) 함수에 의해 버퍼 스와핑을 명령하게 된다.
		glutSwapBuffers();
	}

	// 마우스 클릭 콜백 함수
	void mouseClick(GLint Button, GLint State, GLint X, GLint Y) {
		// 마우스 왼쪽 버튼이 눌러질 때, 해당 좌표를 pX, pY에 저장
		if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
			bPressed = true;
			pX = X;
			pY = Y;
		}
		else
			bPressed = false;
	}

	// 마우스를 움질일 때 호출되는 콜백 함수
	void mouseMove(GLint X, GLint Y) {
		// 마우스 커서 좌표 변화량과 좌표 값을 저장
		dx = X - pX;
		dy = Y - pY;
		pX = X;
		pY = Y;

		// 마우스 왼쪽 버튼이 눌러졌을 때
		if (bPressed) {
			// 메뉴에서 선택한 모드에 맞는 함수를 호출
			switch (inputMsg) {
			case TRANSLATE:
				mouseEventTrans();
				break;
			case ROTATE:
				mouseEventRotate();
				break;
			case SCALE:
				mouseEventScale();
				break;
			default:
				break;
			}
		}
		// 디스플레이 콜백 함수 호출
		glutPostRedisplay();
	}

	// 타이머 콜백
	void timer(int value) {
		// 애니메이션 모드가 ON 상태이면
		if (isAnimationOn) {
			// armUp이 true일 경우 Delta값을 1씩 증가
			// armUp이 false일 경우 Delta값을 1씩 감소
			if (armUp)
				Delta += value;
			else
				Delta -= value;

			// Delta 값이 35를 넘어서면 Delta를 감소모드로
			// Delta 값이 0보다 작으면 Delta를 증가모드로
			if (Delta >= 35)
				armUp = false;
			else if (Delta <= 0)
				armUp = true;

			// 디스플레이 콜백 함수 호출
			glutPostRedisplay();
		}

	}

	// 키보드 콜백 함수
	void keyboard(unsigned char KeyPressed, int X, int Y) {
		/* 해당 키 입력시 다음을 수행
		v 또는 V : modelView 행렬을 도스창에 출력
		q 또는 Q : 프로그램 종료
		*/

		switch (KeyPressed) {
		case 'V': case 'v':
			myMat.printModelViewMatrix();
			break;
		case 'Q': case 'q':
			exit(0);
			break;
		default:
			break;
		}
	}

	// 메뉴 콜백 함수
	void mainMenu(int entryID) {
		inputMsg = entryID;

		// 에니메이션 켜기 메뉴가 선택되면 isAnimationOn을 true로
		// 애니메이션 끄기 메뉴가 선택되면 isAnimationOn을 false로
		if (inputMsg == ANIMATION_ON)
			isAnimationOn = true;
		else if (inputMsg == ANIMATION_OFF)
			isAnimationOn = false;

		// 종료 메뉴가 선택되면 프로그램 종료
		else if (inputMsg == EXIT_PROGRAM)
			exit(0);
	}

	// 현 메뉴에 메뉴 항목을 추가한다.
	void addMenuEntry() {
		glutAddMenuEntry("이동", TRANSLATE);
		glutAddMenuEntry("회전", ROTATE);
		glutAddMenuEntry("크기변화", SCALE);
		glutAddMenuEntry("애니메이션 켜기", ANIMATION_ON);
		glutAddMenuEntry("애니메이션 끄기", ANIMATION_OFF);
		glutAddMenuEntry("No Action", NO_ACTION);
		glutAddMenuEntry("Exit", EXIT_PROGRAM);
		glutAttachMenu(GLUT_RIGHT_BUTTON);  // 오른쪽 클릭을 하면 메뉴가 팝업
	}
};
