#include <cmath>
#include <iostream>
#include <windows.h>
#define Screen_Max_height 31
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
Point Rotate_X(Point P,float degree) {//return Point R rotate Point P by x axes 
	Point R = Point(0,0,0);
	R.x = P.x;
	R.y = P.y * cosf(degree) + P.z * sinf(degree);
	R.z = -P.y * sinf(degree) + P.z * cosf(degree);
	return R;
}; 
Point Rotate_Y(Point P, float degree) {//return Point R rotate Point P by y axes 
	Point R = Point(0, 0, 0);
	R.x = P.x * cosf(degree) + P.z * sinf(degree);
	R.y = P.y;
	R.z = -P.x * sinf(degree) + P.z * cosf(degree);
	return R;
};
Point Rotate_Z(Point P, float degree) {//return Point R rotate Point P by z axes 
	Point R = Point(0, 0, 0);
	R.x = P.x * cosf(degree) + P.y * sinf(degree);
	R.y = -P.x * sinf(degree) + P.y * cosf(degree);
	R.z = P.z;
	return R;
};
void print_donut() {//Print the screen the x,y screen it just well known Cartesian coordinate system
	for (int r = Screen_Max_height - 1; r >= 0 ; r--) {
		for (int c = 0 ; c < Screen_Max_width ; c++) {
			putchar(Frame_buffer[r][c]);
		}
		putchar('\n');
	}
}
float Dot_Product(Point A,Point B) {//dot product for calculating luminance
	float result = A.x * B.x + A.y * B.y + A.z * B.z;
	return result;
}
int main() {
	float xdegree = 0;
	float zdegree = 0;
	int cnt = 0;
	Point init = Point(100, 0, 0);
	float r = 40;
	char Bright[13] = ".,-~:;!=*#$@";//the higher the illumination, we see the point brighter 
	while (1) {
		printf("\x1b[2j");// clean the window
		for (int r = 0; r < Screen_Max_height; r++) {
			for (int c = 0; c < Screen_Max_width; c++) {
				Frame_buffer[r][c] = ' ';
				z_index[r][c] = 0;
			}
		}
		for (float i = 0; i < 6.28; i += 0.07) {//make a circle
			Point P = Point(r * cosf(i) + init.x, r * sinf(i) + init.y, init.z);
			for (float j = 0; j < 6.28 ; j += 0.02) {//rotate to y to make a donut
				Point Y = Rotate_Z(Rotate_X(Rotate_Y(P, j),xdegree),zdegree);
				float depth = 1 / (Y.z+200);//depth for z_index
				int sx = (Y.x * 45)/(Y.z+300);//screen index x
				int sy = (Y.y * 25)/(Y.z+300);//screen index y
				Point Normal_Vector = Point(cosf(i), sinf(i), 0);
				Normal_Vector = Rotate_Z(Rotate_X(Rotate_Y(Normal_Vector, j), xdegree), zdegree);
				float Dot = Dot_Product(Normal_Vector, Point(0, 1, -1));
				int mx = 20;//move to middle point
				int my = 15;//move to middle point
				int Illumination = 8*Dot;
				if (Illumination < 0) Illumination = 0;
				if ( mx+sx >= 0 && mx+sx <= 40 && my+sy >= 0 && my+sy <= 30) {// if the point is in the screen
					if (z_index[my + sy][mx + sx] < depth) {
						z_index[my + sy][mx + sx] = depth;
						Frame_buffer[my + sy][mx + sx] = Bright[Illumination];
					}
				}
			}
		}
		printf("\x1b[H");//stick cusor to the home
		print_donut();//print cur screen
		//rotate x axes and z axes
		xdegree += 0.15; 
		zdegree -= 0.1;
	}
	return 0;
}
