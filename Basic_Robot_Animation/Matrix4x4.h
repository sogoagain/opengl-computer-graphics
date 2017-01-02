#pragma once
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>

class Matrix4x4
{
private:
	float	mat[16];

	// 현재 Opengl의 행렬을 읽어오는 함수
	//	GL_MODELVIEW_MATRIX, GL_PROJECTION_MATRIX, GL_TEXTURE_MATRIX, GL_COLOR_MATRIX
	void getMatrix(GLenum pname) {
		glGetFloatv(pname, mat);
	}

	// 행렬 초기화
	void identity() {
		for (int i = 0; i<4; i++)
			for (int j = 0; j<4; j++) {
				mat[i * 4 + j] = (i == j) ? 1.0f : 0.0f;
			}
	}

	// 행렬 출력 함수
	void printMatrix(char* str = "현재 행렬 = ") {
		printf("%s", str);
		for (int i = 0; i<4; i++) {
			printf("\n\t");
			for (int j = 0; j<4; j++)
				printf("%6.2f", mat[j * 4 + i]);
		}
		printf("\n");
	}
public:

	Matrix4x4(void) { identity(); }
	~Matrix4x4(void) { }

	// 현재 모델뷰 행렬을 출력
	void printModelViewMatrix(char *str = "현재 MODELVIEW 행렬 = ") {
		getMatrix( GL_MODELVIEW_MATRIX );
		printMatrix(str);
	}
};

