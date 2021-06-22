#include <cmath>
#include <iostream>
#include <windows.h>
#define Screen_Max_height 21
#define Screen_Max_width 41
struct Point {
	float x;
	float y;
	float z;
	Point(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}
};
char Frame_buffer[Screen_Max_height][Screen_Max_width] = { ' ', };
float z_index[Screen_Max_height][Screen_Max_width] = { 0, };
Point Rotate_X(Point P,float degree) {
	Point R = Point(0,0,0);
	R.x = P.x;
	R.y = P.y * cos(degree) + P.z * sin(degree);
	R.z = -P.y * sin(degree) + P.z * cos(degree);
	return R;
}; 
Point Rotate_Y(Point P, float degree) {
	Point R = Point(0, 0, 0);
	R.x = P.x * cos(degree) + P.z * sin(degree);
	R.y = P.y;
	R.z = -P.x * sin(degree) + P.z * cos(degree);
	return R;
};
Point Rotate_Z(Point P, float degree) {
	Point R = Point(0, 0, 0);
	R.x = P.x * cos(degree) + P.y * sin(degree);
	R.y = -P.x * sin(degree) + P.y * cos(degree);
	R.z = P.z;
	return R;
};
void print_donut() {
	for (int r = Screen_Max_height - 1; r >= 0 ; r--) {
		printf("%d",r);
		for (int c = 0 ; c < Screen_Max_width ; c++) {
			printf("%c", Frame_buffer[r][c]);
		}
		printf("\n");
	}
}
float Dot_Product(Point A,Point B) {
	float result = A.x * B.x + A.y * B.y + A.z * B.z;
	return result;
}
int main() {
	float xdegree = 0;
	float zdegree = 0;
	Point init = Point(100, 0, 0);
	float r = 60;
	while (1) {
		for (int r = 0; r < Screen_Max_height; r++) {
			for (int c = 0; c < Screen_Max_width; c++) {
				Frame_buffer[r][c] = ' ';
				z_index[r][c] = 0;
			}
		}
		for (float i = 0; i < 6.28; i += 0.07) {//make a circle
			Point P = Point(r * cos(i)+init.x, r * sin(i)+init.y, init.z);
			for (float j = 0; j < 6.28 ; j += 0.02) {//rotate to y to make a donut
				P = Rotate_Z(Rotate_X(Rotate_Y(P, j),xdegree),zdegree);
				float depth = 1 / (P.z+200);
				int sx = (P.x * 25)/(P.z+200);//screen index x
				int sy = (P.y * 25)/(P.z+200);//screen index y
				Point Normal_Vector = Point(cos(i), sin(i), 0);
				Normal_Vector = Rotate_Z(Rotate_X(Rotate_Y(Normal_Vector, j), xdegree), zdegree);
				float Dot = Dot_Product(Normal_Vector, Point(0, 1, -1));
				int mx = 20;
				int my = 10;
				int Illumination = 8*Dot;
				if (Illumination < 0) Illumination = 0;
				if ( mx+sx >= 0 && mx+sx <= 40 && my+sy >= 0 && my+sy <= 20) {
					if (z_index[my + sy][mx + sx] < depth) {
						z_index[my + sy][mx + sx] = depth;
						Frame_buffer[my + sy][mx + sx] = ".,-~:;=!*#$@"[Illumination];
					}
				}
			}
		}
		Sleep(100);
		printf("\x1b[H");
		print_donut();
		xdegree += 0.04;
		break;
	}
	return 0;
}
