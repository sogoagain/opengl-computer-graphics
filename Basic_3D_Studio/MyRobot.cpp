#include <gl/glut.h>
#include "MyRobot.h"

// 로봇의 머리
void drawHead() {
	glPushMatrix(); {
		glTranslatef(0.0, 0.75, 0.0);
		glutWireSphere(0.15, 16, 16);
	} glPopMatrix();
}

// 로봇의 몸통
void drawBody() {
	glPushMatrix(); {
		glTranslatef(0.0, 0.5, 0.0);
		glScalef(3, 1, 1);
		glutWireCube(0.2);
	} glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0.0, 0.4, 0.0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.2, 0.2, 0.4, 16, 16);
	} glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0.0, -0.05, 0.0);
		glScalef(2, 1, 1);
		glutWireCube(0.2);
	} glPopMatrix();
}

// 로봇의 팔과 다리의 첫번째 부분을 그리는 함수
// drawFirstArm의 매개변수 중 fTx, fTy는 x축, y축 이동 거리
// fAngleZ은 z축을 기준으로 회전하는 회전 각도다.
void drawFirstArm(float fTx, float fTy, float fAngleZ) {
	glPushMatrix();
	{
		glTranslatef(fTx, fTy, 0.0);
		glRotatef(fAngleZ, 0.0, 0.0, 1.0);

		glutWireSphere(0.05, 16, 16);
		glTranslatef(0.0, -0.15, 0.0);
		glScalef(1.0, 2.0, 1.0);
		glutWireCube(0.1);
	}
	glPopMatrix();
}

// 로봇의 팔과 다리의 두번째 부분을 그리는 함수
// drawSecondArm의 매개변수 중 fTx, fTy는 x축, y축 이동 거리
// fAngleZ은 z축을 기준으로 회전하는 회전 각도,
// fAnfleX는 x축을 기준으로 회전하는 회전 각도
void drawSecondArm(float fTx, float fTy, float fAngleZ, float fAngleX) {
	glPushMatrix();
	{
		glTranslatef(fTx, fTy, 0.0);
		glRotatef(fAngleZ, 0.0, 0.0, 1.0);
		glRotatef(fAngleX, 1.0, 0.0, 0.0);

		glutWireSphere(0.05, 16, 16);
		glTranslatef(0.0, -0.25, 0.0);
		glScalef(1.0, 2.0, 1.0);
		glutWireCube(0.2);
	}
	glPopMatrix();
}

// 로봇의 머리, 몸통, 팔, 다리를 합친다.
// 특히 팔과 다리는 적절한 매개변수를 이용하여 그린다.
void drawMyRobot() {
	// 머리
	drawHead();
	// 몸통
	drawBody();
	// 오른팔
	drawFirstArm(0.35, 0.55, 45);
	drawSecondArm(0.55, 0.3, 0, -45);
	// 왼팔
	drawFirstArm(-0.35, 0.55, -90);
	drawSecondArm(-0.65, 0.55, -45, -10);
	// 오른다리
	drawFirstArm(0.25, -0.15, 0);
	drawSecondArm(0.25, -0.45, 0, 30);
	// 왼다리
	drawFirstArm(-0.25, -0.15, 0);
	drawSecondArm(-0.25, -0.45, 0, -25);
}