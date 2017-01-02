#include <stdlib.h>
#include <gl/glut.h>
#include "MyRobot.h"

#define ORTHO_SIZE 2.0

int gWidth, gHeight;	// 현재 윈도우의 창 크기
int gMenuChoice;		// 현재 선택된 메뉴항목
int gClickedPosition;	// 마우스가 클릭되었을 때 커서의 위치
int gPx, gPy;			// 이전 마우스 커서의 위치
int	gDx, gDy;			// 마우스 커서 위치의 변화량
float gTx, gTy, gTz;	// 전역좌표계 기준으로 물체의 x축, y축, z축으로의 이동량
int	gRx, gRy, gRz, gRm;	// Rx: x축을 기준으로 회전시켰을때의 회전량
						// Ry: y축을 기준으로 회전시켰을때의 회전량 
						// Rz: z축을 기준으로 회전시켰을때의 회전량 
						// Rm: (1,0,-1)축을 기준으로 회전시켰을때의 회전량 
int	gSx;				// 크기 변화량

// 메뉴 아이템의 상수 값
enum MenuItem {
	TRANSLATE,
	ROTATE,
	SCALE,
	ORIGINALLY,
	EXIT
};

// 화면 분할값
enum SplitScreen {
	VIEWPORT_1,
	VIEWPORT_2,
	VIEWPORT_3,
	VIEWPORT_4
};

// 좌표축을 그리는 함수
void drawAxes(void) {
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.75, 0.25, 0.0);
	glVertex3f(0.75, -0.25, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.75, 0.0, 0.25);
	glVertex3f(0.75, 0.0, -0.25);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.75, 0.25);
	glVertex3f(0.0, 0.75, -0.25);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.25, 0.75, 0.0);
	glVertex3f(-0.25, 0.75, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.25, 0.0, 0.75);
	glVertex3f(-0.25, 0.0, 0.75);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.25, 0.75);
	glVertex3f(0.0, -0.25, 0.75);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();

	glColor3ub(0, 0, 255);
	glRasterPos3f(1.1, 0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
	glRasterPos3f(0.0, 1.1, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
	glRasterPos3f(0.0, 0.0, 1.1);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
}

// 회색 바닥을 그리는 함수
void drawFloor(void) {
	glColor3ub(204, 204, 204);
	glPushMatrix(); {
		glTranslatef(0.0, -1.0, 0.0);
		glBegin(GL_QUADS); {
			glVertex3f(2.0, 0.0, 2.0);
			glVertex3f(2.0, 0.0, -2.0);
			glVertex3f(-2.0, 0.0, -2.0);
			glVertex3f(-2.0, 0.0, 2.0);
		} glEnd();
	} glPopMatrix();
}

// 화면에 출력되는 요소들을 모두 합쳐서 그리는 함수.
void drawScene(void) {
	// 바닥을 그린다.
	drawFloor();

	// 좌표계를 그린다.
	glPushMatrix(); {
		glTranslatef(-1.0, 1.0, -1.0);
		glScalef(0.5, 0.5, 0.5);
		drawAxes();
	} glPopMatrix();

	/*		< 마우스 이벤트에 대한 처리 >
	 * gTx, gTy, gTz만큼 물체를 이동시킨다.
	 * x축, y축, z축을 기준으로 gRx, gRy, gRz 만큼 물체를 회전시킨다.
	 * (1,0,-1) 축을 기준으로 gRm 만큼 물체를 회전시킨다.
	 * 1 + gSx/10 만큼 물체를 확대한다. 단, 0.1보다 더 작게 확대할 수는 없다.
	 */
	glTranslatef(gTx, -gTy, gTz);
	glRotatef(gRx, 1.0, 0.0, 0.0);
	glRotatef(gRy, 0.0, 1.0, 0.0);
	glRotatef(gRz, 0.0, 0.0, 1.0);
	glRotatef(gRm, 1.0, 0.0, -1.0);
	if (1 + gSx / 10. >= 0.1)
		glScalef(1 + gSx / 10., 1 + gSx / 10., 1 + gSx / 10.);
	else
		glScalef(0.1, 0.1, 0.1);

	// 로봇을 그린다.
	glColor3ub(153, 000, 153);	// 로봇의 색은 보라색
	drawMyRobot();
}

// 모델 뷰 행렬과 투상 행렬을 초기화한다.
void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// 행렬 모드를 투상 행렬로 설정한 뒤 항등행렬로 초기화한다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 기본 투상 방법을 평행 투상으로 설정한다.
	glOrtho(-ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE);

	// 행렬 모드를 모델뷰 행렬로 설정한 뒤 항등 행렬로 초기화한다.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// 디스플레이 콜백 함수
void displayFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	/*
	 *			< 1번 뷰포트 >
	 * 우상단 뷰 포트로서 물체를 원근 투상으로 보여준다.
	 */
	glViewport(gWidth / 2, gHeight / 2, gWidth / 2, gHeight / 2); {
		// Init() 함수에서 설정한 평행 투상 대신 원근 투상을 적용하기 위해
		// 먼저 glMatrixMode() 함수에 의해 행렬 모드를 투상 행렬로 놓는다.
		glMatrixMode(GL_PROJECTION);

		glPushMatrix(); {
			glLoadIdentity();		// 투상 행렬을 항등 행렬로 초기화한다.
			// 원근 투상의 가시 부피를 설정한다.
			// 시야각 30도, 종횡비 1.0, 전방 절단면 0.5, 후방 절단면 50에 위치한다.
			gluPerspective(30, 1.0, 3.0, 50.0);
			// 행렬 모드를 모델 뷰 행렬로 바꿔 시점 좌표계를 설정한다.
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix(); {
				// 전역 좌표계를 기준으로 시점 좌표계를 설정한다.
				// 설정한 시점(5.0, 5.0, 5.0)에서 원점(0.0, 0.0, 0.0)을 향하는 -z축 방향으로 3.0 거리에 있다.
				gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
				drawScene();
			} glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		} glPopMatrix();
	}

	/*
	 *			< 2번 뷰포트 >
	 * 좌상단 뷰 포트로서 물체를 평행 투상으로 보여준다.
	 * +y축에서 내려다 본 평면도가 그려진다.
	 */
	glViewport(0, gHeight / 2, gWidth / 2, gHeight / 2); {
		// 행렬 모드를 명시하지 않았을 때 기본적으로 불려오는 것은 모델 뷰 행렬이다.
		// 모델 뷰 행렬이 푸시 함수에 의해 저장된다.
		glPushMatrix(); {
			// 카메라 위치 (0.0, 1.0, 0.0)
			// 바라보는 곳 (0.0, 0.0, 0.0)
			// 카메라 상향 벡터 (0.0, 0.0, -1.0)
			gluLookAt(0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
			drawScene();
		} glPopMatrix();
	}

	/*
	 *			< 3번 뷰포트 >
	 * 좌하단 뷰 포트로서 물체를 평행 투상으로 보여준다.
	 * +z축에서 본 측면도가 그려진다.
	 */
	glViewport(0, 0, gWidth / 2, gHeight / 2); {
		glPushMatrix(); {
			gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
			drawScene();
		} glPopMatrix();
	}

	/*
	 *			< 4번 뷰포트 >
	 * 우하단 뷰 포트로서 물체를 평행 투상으로 보여준다.
	 * +x축에서 본 정면도가 그려진다.
	 */
	glViewport(gWidth / 2, 0, gWidth / 2, gHeight / 2); {
		glPushMatrix(); {
			gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
			drawScene();
		} glPopMatrix();
	}

	glFlush();
}

// 리셰이프 콜백 함수
void reshapeFunc(int w, int h) {
	// 바뀐 윈도우의 크기를 받아온다.
	gWidth = w;
	gHeight = h;
}

// 마우스 클릭 콜백 함수
void mouseClickFunc(GLint button, GLint state, GLint x, GLint y) {
	// 마우스 왼쪽 버튼이 눌러질 때, 해당 좌표를 gPx, gPy에 저장
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		gPx = x;
		gPy = y;

		// 마우스가 클릭되었을 때 마우스 커서의 위치를 받아온다.
		// 마우스 커서의 위치는 뷰포트 단위로 끊는다.
		if (x > gWidth / 2 && y <= gHeight / 2) {
			gClickedPosition = VIEWPORT_1;
		}
		else if (x <= gWidth / 2 && y <= gHeight / 2) {
			gClickedPosition = VIEWPORT_2;
		}
		else if (x <= gWidth / 2 && y > gHeight / 2) {
			gClickedPosition = VIEWPORT_3;
		}
		else
			gClickedPosition = VIEWPORT_4;
	}
}

// 마우스 모션 콜백 함수
void mouseMotionFunc(GLint x, GLint y) {
	// 마우스 커서의 좌표 변화량과 좌표 값을 저장
	gDx = x - gPx;
	gDy = y - gPy;
	gPx = x;
	gPy = y;

	// 현재 물체를 이동시켜야한다면
	if (gMenuChoice == TRANSLATE) {
		float fRatio = 2 * ORTHO_SIZE * ORTHO_SIZE;
		// 마우스 커서의 위치별로 gTx, gTy, gTz를 설정한다.
		switch (gClickedPosition) {
		case VIEWPORT_1:
			gTx += gDx*fRatio / gWidth;
			gTz -= gDx*fRatio / gWidth;
			gTy += gDy*fRatio / gHeight;
			break;
		case VIEWPORT_2:
			gTx += gDx*fRatio / gWidth;
			gTz += gDy*fRatio / gHeight;
			break;
		case VIEWPORT_3:
			gTx += gDx*fRatio / gWidth;
			gTy += gDy*fRatio / gHeight;
			break;
		case VIEWPORT_4:
			gTz -= gDx*fRatio / gWidth;
			gTy += gDy*fRatio / gHeight;
			break;
		}
	}
	// 현재 물체를 회전시켜야 한다면
	else if (gMenuChoice == ROTATE) {
		// 마우스 커서의 위치별로 gRx, gRy, gRz, gRm을 설정한다.
		switch (gClickedPosition) {
		case VIEWPORT_1:
			gRm += gDy;	// gRm은 (1,0,-1)축으로 회전시키는 회전량이다.
						// 뷰포트 1에서 마우스를 y축으로 움직였을 때
						// 물체가 (1,0,-1)축으로 회전되어야 하기 때문이다.
			gRy += gDx;
			break;
		case VIEWPORT_2:
			gRx += gDy;
			gRz -= gDx;
			break;
		case VIEWPORT_3:
			gRx += gDy;
			gRy += gDx;
			break;
		case VIEWPORT_4:
			gRy += gDx;
			gRz -= gDy;
			break;
		}
	}
	// 현재 물체를 확대해야 한다면 gSx값을 설정한다.
	else if (gMenuChoice == SCALE) {
		gSx += gDx;
	}

	// 디스플레이 콜백함수 호출
	glutPostRedisplay();
}

void keyboardFunc(unsigned char uChKeyPressed, int x, int y) {
	/* 해당 키 입력시 다음을 수행
	 * q 또는 Q : 프로그램 종료
	 */

	switch (uChKeyPressed) {
	case 'Q': case 'q':
		exit(0);
		break;
	default:
		break;
	}
}

// 메뉴 콜백 함수
void selectMenuFunc(int entryID) {
	// 선택된 메뉴 아이템을 받아온다.
	gMenuChoice = entryID;

	// 원래대로가 선택되면 객체를 옮기고, 회전시키고, 크기변환한 값을 0으로 초기화한다.
	if (gMenuChoice == ORIGINALLY) {
		gTx = gTy = gTz = gRx = gRy = gRz = gRm = gSx = 0;
		// 디스플레이 콜백함수 호출
		glutPostRedisplay();
	}
	// 종료 메뉴가 선택되면 프로그램을 종료한다
	else if (gMenuChoice == EXIT)
		exit(0);
}

// 현 메뉴에 메뉴 항목을 추가한다.
void addMenuEntry() {
	glutAddMenuEntry("이동", TRANSLATE);
	glutAddMenuEntry("회전", ROTATE);
	glutAddMenuEntry("크기변화", SCALE);
	glutAddMenuEntry("원래대로", ORIGINALLY);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);  // 오른쪽 클릭을 하면 메뉴가 팝업
}

int main(int argc, char** argv) {
	// 윈도우 설정을 초기화한다.
	glutInit(&argc, argv);
	gWidth = 500;
	gHeight = 500;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(gWidth, gHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("My 3D Studio");

	// 모델 뷰행렬과 투상행렬에 대한 초기 설정을한다.
	Init();

	// 콜백 함수를 등록한다.
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutMouseFunc(mouseClickFunc);
	glutMotionFunc(mouseMotionFunc);
	glutKeyboardFunc(keyboardFunc);

	// 메뉴 콜백함수 등록하고 메뉴 항목을 추가한다.
	GLint MyMainMenuID = glutCreateMenu(selectMenuFunc);
	addMenuEntry();
	gMenuChoice = ORIGINALLY;

	glutMainLoop();
	return 0;
}