#include <GL/glut.h>

// 로봇 몸통
void drawBody()
{
	glPushMatrix();
	{
		glutWireCube (0.8);
	}
	glPopMatrix();
}

// 로봇 머리
void drawHead()
{
	glPushMatrix();
	{
		glTranslatef(0.18, 0.6, 0.0);
		glutWireSphere(0.15, 16, 16);
	}
	glPopMatrix();

		glPushMatrix();
	{
		glTranslatef(-0.18, 0.6, 0.0);
		glutWireSphere(0.15, 16, 16);
	}
	glPopMatrix();
}

// 로봇 다리
void drawLeg()
{
	glPushMatrix();
	{
		glTranslatef(0.55, -0.35, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glutWireTorus(0.1, 0.4, 16, 16);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-0.55, -0.35, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glutWireTorus(0.1, 0.4, 16, 16);
	}
	glPopMatrix();
}

// 로봇 팔
// 타이머에서 Delta변수로 Rotatef를 이용해 컨트롤
void drawArm(GLfloat Delta)
{
	glPushMatrix();
	{
		glRotatef(-25.0+Delta, 1.0, 0.0, 0.0);
		glTranslatef(0.5, 0.4, 0.4);
		glScalef(1.0, 1.0, 6.0);
		glutWireCube (0.15);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glRotatef(15.0-Delta, 1.0, 0.0, 0.0);
		glTranslatef(-0.5, 0.4, 0.4);
		glScalef(1.0, 1.0, 6.0);
		glutWireCube (0.15);
	}
}

// 몸통, 머리, 다리, 팔 조립하여 출력
void drawMyRobot(GLfloat Delta)
{
	glPushMatrix();
	{
		drawBody();
		drawHead();
		drawLeg();
		drawArm(Delta);
	}
	glPopMatrix();
}
