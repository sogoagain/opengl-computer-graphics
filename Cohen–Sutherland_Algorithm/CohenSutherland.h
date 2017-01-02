#include <gl/glut.h>

// TRUE는 1, FALSE는 0
#define FALSE 0
#define TRUE 1

/* 
4비트 아웃코드의 bit masks다.
왼쪽 절단면을 0001, 오른쪽 절단면을 0010
하단 절단면을 0100, 상단 절단면을 1000으로 정한다.
절단면 밖이라면 각 비트의 값은 1,
절단면 안이라면 각 비트의 값은 0.
*/
#define LEFT_EDGE 0x1		// 0001
#define RIGHT_EDGE 0x2		// 0010
#define BOTTOM_EDGE 0x4		// 0100
#define TOP_EDGE 0x8		// 1000

/*		<INSIDE(a)>
	한 점의 아웃코드를 판별한다.
	한 점의 아웃코드에 NOT을 취해 1이 반환되면 아웃코드가 0000이었던 것이므로 그려질 영역 안에 있는 것이다.
	만일, 0이 반환되면 그려질 영역 밖에 있는 것이다.
*/
#define INSIDE(a) (!a)

/*		REJECT(a,b)
	두 점의 아웃코드에 &연산을 취해 선분이 그려질 영역 밖에 있는지 판별한다.
	만일 두 점의 아웃코드를 AND 연산했을때 결과값에 1이 하나라도 있다면
	두 점이 같은 절단면 밖에 있으므로 그려질 영역 밖에 있는 것이므로 무시한다.
*/
#define REJECT(a,b) (a&b)

/*		<ACCEPT(a,b)>	
	두 점의 아웃코드를 OR 연산한뒤 NOT 연산을 한다.
	만일 한점이라도 사각형 영역 밖에 있다면	OR 연산의 결과가 0이 아닐것이다.
	즉, 0이 아닌 값에 NOT을 취한 결과 0이 반환값이다.
	만일 두점모두 사각형 영역 안에 있다면 OR 연산의 결과 0이 나온다.
	즉, 0에 NOT을 취한 결과 1이 반환값이다.
	두 점이 포함되어 있는 아웃코드가 모두 0000일때 그려질 영역에 있는 것이다.
	그 외의 경우 적어도 한점은 그려질 영역 밖에 존재한다.
	따라서 두 점의 아웃코드를 OR 연산 하였을 때 0000이라면 클리핑이 완료된 것이다.
*/
#define ACCEPT(a,b) (!(a|b))

// 점을 나타내는 구조체 Point, GLfloat형의 x와 y를 멤버변수로 갖는다.
struct Point {
	GLfloat x;
	GLfloat y;
};

// 점의 아웃코드를 계산한다.
// 그려질 영역의 좌하단, 우상단 좌표와 아웃코드를 구할 점을 매개변수로 받는다.
unsigned char encode (Point pt,
	Point winMin, Point winMax)
{
	unsigned char code=0x00;
	if (pt.x < winMin.x)
		code = code | LEFT_EDGE;
	if (pt.x > winMax.x)
		code = code | RIGHT_EDGE;
	if (pt.y < winMin.y)
		code = code | BOTTOM_EDGE;
	if (pt.y > winMax.y)
		code = code | TOP_EDGE;
	return (code);
}

// 두 점의 좌표를 교환한다.
void swapPts (Point * p1, Point * p2)
{
	Point tmp;
	tmp = *p1; *p1 = *p2; *p2 = tmp;
}

// 두 점의 아웃코드를 교환한다.
void swapCodes (unsigned char * c1,
	unsigned char * c2)
{
	unsigned char tmp;
	tmp = *c1; *c1 = *c2; *c2 = tmp;
}

// 선분을 클리핑 한다.
// 그려질 영역의 좌하단, 우상단 좌표와
// 선분의 시작점, 끝점을 매개변수로 받는다.
void clipLine (Point winMin, Point winMax, Point p1, Point p2)
{
	unsigned char code1, code2;		// 선분의 끝점의 계산된 아웃코드다.
	int done = FALSE, draw = FALSE;	// done이 true라면 해당 선분에 대해서는 더이상 고려할 필요가 없다.
									// draw가 true라면 해당 선분은 그린다. false라면 온전히 사각형 밖에 있으므로 그리지 않는다.
	float m;	// 선분의 기울기

	// done이 true가 될 때 까지(클리핑이 완료될 때 까지) 반복문을 돌리며 좌표를 계산한다.
	while (!done) {
		// 선분의 끝점에 대해 아웃코드를 계산한다.
		code1 = encode (p1, winMin, winMax);
		code2 = encode (p2, winMin, winMax);

		// ACCEPT 연산을 통해 클리핑이 완료되었는지 판단한다.
		// 선분이 그려질 영역 안에 존재한다면 클리핑을 끝내고 그린다.
		if (ACCEPT (code1, code2)) {
			done = TRUE;
			draw = TRUE;
		}

		// 선분이 그려질 영역 밖에 있다면 클리핑을 끝내고 그리지않는다.
		else if (REJECT (code1, code2))
			done = TRUE;

		// 적어도 한점이 그려질 영역 밖에 있고
		// 선분이 그려져야할 영역이 있을수 있다면 교차점 계산을 한다.
		else {
			// 시작점이 그려질 영역 안에 있는 지 확인한다.
			// 만일 시작점이 그려질 영역 안에 있다면 시작점과 끝점을 바꾼다.
			// 즉 시작점 p1을 기준으로 교차점 계산을 한다.
			if (INSIDE (code1)) {
				swapPts (&p1, &p2);
				swapCodes (&code1, &code2);
			}

			// 교차점 계산을 위해 기울기 m을 구한다.
			// p1 좌표를 교차점 계산에 의해 구한다.
			if (p2.x != p1.x) m = (p2.y - p1.y) / (p2.x - p1.x);
			if (code1 & LEFT_EDGE) {
				p1.y += (winMin.x - p1.x) * m;
				p1.x = winMin.x;
			}
			else if (code1 & RIGHT_EDGE) {
				p1.y += (winMax.x - p1.x) * m;
				p1.x = winMax.x;
			}
			else if (code1 & BOTTOM_EDGE) {
				/* Need to update p1.x for non-vertical lines only */
				if (p2.x != p1.x)
					p1.x += (winMin.y - p1.y) / m;
				p1.y = winMin.y;
			}
			else if (code1 & TOP_EDGE) {
				if (p2.x != p1.x) p1.x += (winMax.y - p1.y) / m;
				p1.y = winMax.y;
			}
		}
	}

	// 클리핑이 완료되고 그릴 것이 있다면 GL_GLINE_LOOP을 이용해 선분을 그린다.
	if (draw) {
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.x, p1.y, 0);
		glVertex3f(p2.x, p2.y, 0);
		glEnd();

		printf("클리핑된 P1: (%5.2f,%5.2f)\n", p1.x, p1.y);
		printf("클리핑된 P2: (%5.2f,%5.2f)\n", p2.x, p2.y);
	}
}

