// main.cpp
// Developed by PDS Group @ CEID Fall 2009
// Based on the code of Richard S. Wright Jr.
// from the book "OpenGL SuperBible"

#include <pthread.h>
#define FREEGLUT_STATIC

// Bring in OpenGL 
// Mac OS X
#ifdef __APPLE__
#include <Carbon/Carbon.h>	// Brings in most Apple specific stuff
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/glu.h>		// OpenGL Utilities
#include <Glut/glut.h>		// Apples Implementation of GLUT
#include <unistd.h>
#endif

#ifdef linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#endif

#include <math.h>
#include "main.h"
//include the file of time library.
//#include <time.h>
#include <sys/time.h>
#define TEMPRANGE 100.0
#define ROOMTEMP 0.0
#define SOURCETEMP 100.0

#define SOURCE 0
#define PLATE 1

#define FALSE 0
#define TRUE 1

const double EPSILON = 0.0000000001;

// Rotation amounts
GLdouble sourceRot[16];
GLdouble plateRot[16];

static int refresh = 100; 	// In milliseconds
static int object = PLATE; 
static int benchmark = TRUE;  //Interactive or benchmark mode
static int finish = FALSE;
static int maxrepeats = 1;  //Benchmark mode

// Plate temperatures for the jacobi calculation
volatile double ***before;
volatile double ***after;

// x-axis
const int cols = 240;
// y-axis
const int rows = 80;
// z-axis
const int faces = 8;

// Size of each "atom" cube
const double size = 0.4;

// Atom cube coordinates. Will be multiplied lates
// appropriatelly for each "atom" cube
static int xcenter;
static int ycenter;
static int zcenter;

//Heat source sphere coordinates.
double xsource = (cols + 8) * size;
double ysource = (rows + 8) * size;
double zsource = (faces + 8) * size;

//For the benchmark mode
int isrc = 0;
const int lensrc = 8;
//const double xsrc[] = {xsource, xsource, xsource, xsource, -xsource, -xsource, -xsource, -xsource};
const double xsrc[] = {xsource, -xsource, xsource, -xsource, xsource, -xsource, xsource, -xsource};
const double ysrc[] = {ysource, ysource, -ysource, -ysource, ysource, ysource, -ysource, -ysource}; 
//const double zsrc[] = {zsource, -zsource, zsource, -zsource, zsource, -zsource, zsource, -zsource};
const double zsrc[] = {zsource, zsource, zsource, zsource, -zsource, -zsource, -zsource, -zsource};

// Rotation values for SOURCE (0) and PLATE (1)
static GLdouble xRot[2] = {0.0f, 0.0f};
static GLdouble yRot[2] = {0.0f, 0.0f};
static GLdouble zRot[2] = {0.0f, 0.0f};

// Translation values for SOURCE (0) and PLATE (1) 
static GLdouble xMove[2] = {xsource, xsource};
static GLdouble yMove[2] = {ysource, ysource};
static GLdouble zMove[2] = {zsource, zsource};

// 500 x 3
GLubyte palette[][3] = {
{0, 0, 255}, {0, 0, 254}, {0, 0, 253}, {0, 0, 252}, {0, 0, 250},
{0, 0, 249}, {0, 0, 248}, {0, 0, 247}, {0, 0, 246}, {0, 0, 245},
{0, 0, 243}, {0, 0, 242}, {0, 0, 241}, {0, 0, 240}, {0, 0, 239},
{0, 0, 238}, {0, 0, 236}, {0, 0, 235}, {0, 0, 234}, {0, 0, 233},
{0, 0, 232}, {0, 0, 231}, {0, 0, 229}, {0, 0, 228}, {0, 0, 227},
{0, 0, 226}, {0, 0, 225}, {0, 0, 224}, {0, 0, 222}, {0, 0, 221},
{0, 0, 220}, {0, 0, 219}, {0, 0, 218}, {0, 0, 217}, {0, 0, 215},
{0, 0, 214}, {0, 0, 213}, {0, 0, 212}, {0, 0, 211}, {0, 0, 210},
{0, 0, 209}, {0, 0, 207}, {0, 0, 206}, {0, 0, 205}, {0, 0, 204},
{0, 0, 203}, {0, 0, 202}, {0, 0, 200}, {0, 0, 199}, {0, 0, 198},
{0, 0, 197}, {0, 0, 196}, {0, 0, 195}, {0, 0, 193}, {0, 0, 192},
{0, 0, 191}, {0, 0, 190}, {0, 0, 189}, {0, 0, 188}, {0, 0, 186},
{0, 0, 185}, {0, 0, 184}, {0, 0, 183}, {0, 0, 182}, {0, 0, 181},
{0, 0, 179}, {0, 0, 178}, {0, 0, 177}, {0, 0, 176}, {0, 0, 175},
{0, 0, 174}, {0, 0, 172}, {0, 0, 171}, {0, 0, 170}, {0, 0, 169},
{0, 0, 168}, {0, 0, 167}, {0, 0, 166}, {0, 0, 164}, {0, 0, 163},
{0, 0, 162}, {0, 0, 161}, {0, 0, 160}, {0, 0, 159}, {0, 0, 157},
{0, 0, 156}, {0, 0, 155}, {0, 0, 154}, {0, 0, 153}, {0, 0, 152},
{0, 0, 150}, {0, 0, 149}, {0, 0, 148}, {0, 0, 147}, {0, 0, 146},
{0, 0, 145}, {0, 0, 143}, {0, 0, 142}, {0, 0, 141}, {0, 0, 140},
{0, 0, 139}, {2, 0, 137}, {3, 0, 136}, {4, 0, 135}, {6, 0, 133},
{7, 0, 132}, {9, 0, 130}, {10, 0, 129}, {11, 0, 128}, {13, 0, 126},
{14, 0, 125}, {16, 0, 123}, {17, 0, 122}, {18, 0, 121}, {20, 0, 119},
{21, 0, 118}, {23, 0, 116}, {24, 0, 115}, {25, 0, 114}, {27, 0, 112},
{28, 0, 111}, {30, 0, 109}, {31, 0, 108}, {32, 0, 107}, {34, 0, 105},
{35, 0, 104}, {36, 0, 103}, {38, 0, 101}, {39, 0, 100}, {41, 0, 98},
{42, 0, 97}, {43, 0, 96}, {45, 0, 94}, {46, 0, 93}, {48, 0, 91},
{49, 0, 90}, {50, 0, 89}, {52, 0, 87}, {53, 0, 86}, {55, 0, 84},
{56, 0, 83}, {57, 0, 82}, {59, 0, 80}, {60, 0, 79}, {62, 0, 77},
{63, 0, 76}, {64, 0, 75}, {66, 0, 73}, {67, 0, 72}, {69, 0, 70},
{70, 0, 69}, {71, 0, 68}, {73, 0, 66}, {74, 0, 65}, {75, 0, 64},
{77, 0, 62}, {78, 0, 61}, {80, 0, 59}, {81, 0, 58}, {82, 0, 57},
{84, 0, 55}, {85, 0, 54}, {87, 0, 52}, {88, 0, 51}, {89, 0, 50},
{91, 0, 48}, {92, 0, 47}, {94, 0, 45}, {95, 0, 44}, {96, 0, 43},
{98, 0, 41}, {99, 0, 40}, {101, 0, 38}, {102, 0, 37}, {103, 0, 36},
{105, 0, 34}, {106, 0, 33}, {108, 0, 31}, {109, 0, 30}, {110, 0, 29},
{112, 0, 27}, {113, 0, 26}, {114, 0, 25}, {116, 0, 23}, {117, 0, 22},
{119, 0, 20}, {120, 0, 19}, {121, 0, 18}, {123, 0, 16}, {124, 0, 15},
{126, 0, 13}, {127, 0, 12}, {128, 0, 11}, {130, 0, 9}, {131, 0, 8},
{133, 0, 6}, {134, 0, 5}, {135, 0, 4}, {137, 0, 2}, {138, 0, 1},
{139, 0, 0}, {141, 0, 0}, {142, 0, 0}, {143, 0, 0}, {144, 0, 0},
{145, 0, 0}, {146, 0, 0}, {148, 0, 0}, {149, 0, 0}, {150, 0, 0},
{151, 0, 0}, {152, 0, 0}, {153, 0, 0}, {155, 0, 0}, {156, 0, 0},
{157, 0, 0}, {158, 0, 0}, {159, 0, 0}, {160, 0, 0}, {162, 0, 0},
{163, 0, 0}, {164, 0, 0}, {165, 0, 0}, {166, 0, 0}, {167, 0, 0},
{169, 0, 0}, {170, 0, 0}, {171, 0, 0}, {172, 0, 0}, {173, 0, 0},
{174, 0, 0}, {175, 0, 0}, {177, 0, 0}, {178, 0, 0}, {179, 0, 0},
{180, 0, 0}, {181, 0, 0}, {182, 0, 0}, {184, 0, 0}, {185, 0, 0},
{186, 0, 0}, {187, 0, 0}, {188, 0, 0}, {189, 0, 0}, {191, 0, 0},
{192, 0, 0}, {193, 0, 0}, {194, 0, 0}, {195, 0, 0}, {196, 0, 0},
{198, 0, 0}, {199, 0, 0}, {200, 0, 0}, {201, 0, 0}, {202, 0, 0},
{203, 0, 0}, {205, 0, 0}, {206, 0, 0}, {207, 0, 0}, {208, 0, 0},
{209, 0, 0}, {210, 0, 0}, {212, 0, 0}, {213, 0, 0}, {214, 0, 0},
{215, 0, 0}, {216, 0, 0}, {217, 0, 0}, {219, 0, 0}, {220, 0, 0},
{221, 0, 0}, {222, 0, 0}, {223, 0, 0}, {224, 0, 0}, {225, 0, 0},
{227, 0, 0}, {228, 0, 0}, {229, 0, 0}, {230, 0, 0}, {231, 0, 0},
{232, 0, 0}, {234, 0, 0}, {235, 0, 0}, {236, 0, 0}, {237, 0, 0},
{238, 0, 0}, {239, 0, 0}, {241, 0, 0}, {242, 0, 0}, {243, 0, 0},
{244, 0, 0}, {245, 0, 0}, {246, 0, 0}, {248, 0, 0}, {249, 0, 0},
{250, 0, 0}, {251, 0, 0}, {252, 0, 0}, {253, 0, 0}, {255, 0, 0},
{255, 1, 0}, {255, 3, 0}, {255, 4, 0}, {255, 6, 0}, {255, 8, 0},
{255, 9, 0}, {255, 11, 0}, {255, 13, 0}, {255, 14, 0}, {255, 16, 0},
{255, 18, 0}, {255, 19, 0}, {255, 21, 0}, {255, 22, 0}, {255, 24, 0},
{255, 26, 0}, {255, 27, 0}, {255, 29, 0}, {255, 31, 0}, {255, 32, 0},
{255, 34, 0}, {255, 36, 0}, {255, 37, 0}, {255, 39, 0}, {255, 41, 0},
{255, 42, 0}, {255, 44, 0}, {255, 46, 0}, {255, 47, 0}, {255, 49, 0},
{255, 51, 0}, {255, 52, 0}, {255, 54, 0}, {255, 56, 0}, {255, 57, 0},
{255, 59, 0}, {255, 61, 0}, {255, 62, 0}, {255, 64, 0}, {255, 65, 0},
{255, 67, 0}, {255, 69, 0}, {255, 70, 0}, {255, 72, 0}, {255, 74, 0},
{255, 75, 0}, {255, 77, 0}, {255, 79, 0}, {255, 80, 0}, {255, 82, 0},
{255, 84, 0}, {255, 85, 0}, {255, 87, 0}, {255, 89, 0}, {255, 90, 0},
{255, 92, 0}, {255, 94, 0}, {255, 95, 0}, {255, 97, 0}, {255, 99, 0},
{255, 100, 0}, {255, 102, 0}, {255, 103, 0}, {255, 105, 0}, {255, 107, 0},
{255, 108, 0}, {255, 110, 0}, {255, 112, 0}, {255, 113, 0}, {255, 115, 0},
{255, 117, 0}, {255, 118, 0}, {255, 120, 0}, {255, 122, 0}, {255, 123, 0},
{255, 125, 0}, {255, 127, 0}, {255, 128, 0}, {255, 130, 0}, {255, 132, 0},
{255, 133, 0}, {255, 135, 0}, {255, 137, 0}, {255, 138, 0}, {255, 140, 0},
{255, 142, 0}, {255, 143, 0}, {255, 145, 0}, {255, 146, 0}, {255, 148, 0},
{255, 150, 0}, {255, 151, 0}, {255, 153, 0}, {255, 155, 0}, {255, 156, 0},
{255, 158, 0}, {255, 160, 0}, {255, 161, 0}, {255, 163, 0}, {255, 165, 0},
{255, 166, 0}, {255, 167, 0}, {255, 168, 0}, {255, 168, 0}, {255, 169, 0},
{255, 170, 0}, {255, 171, 0}, {255, 172, 0}, {255, 173, 0}, {255, 174, 0},
{255, 175, 0}, {255, 176, 0}, {255, 177, 0}, {255, 177, 0}, {255, 178, 0},
{255, 179, 0}, {255, 180, 0}, {255, 181, 0}, {255, 182, 0}, {255, 183, 0},
{255, 184, 0}, {255, 185, 0}, {255, 186, 0}, {255, 186, 0}, {255, 187, 0},
{255, 188, 0}, {255, 189, 0}, {255, 190, 0}, {255, 191, 0}, {255, 192, 0},
{255, 193, 0}, {255, 194, 0}, {255, 195, 0}, {255, 195, 0}, {255, 196, 0},
{255, 197, 0}, {255, 198, 0}, {255, 199, 0}, {255, 200, 0}, {255, 201, 0},
{255, 202, 0}, {255, 203, 0}, {255, 204, 0}, {255, 204, 0}, {255, 205, 0},
{255, 206, 0}, {255, 207, 0}, {255, 208, 0}, {255, 209, 0}, {255, 210, 0},
{255, 211, 0}, {255, 212, 0}, {255, 213, 0}, {255, 214, 0}, {255, 214, 0},
{255, 215, 0}, {255, 216, 0}, {255, 217, 0}, {255, 218, 0}, {255, 219, 0},
{255, 220, 0}, {255, 221, 0}, {255, 222, 0}, {255, 223, 0}, {255, 223, 0},
{255, 224, 0}, {255, 225, 0}, {255, 226, 0}, {255, 227, 0}, {255, 228, 0},
{255, 229, 0}, {255, 230, 0}, {255, 231, 0}, {255, 232, 0}, {255, 232, 0},
{255, 233, 0}, {255, 234, 0}, {255, 235, 0}, {255, 236, 0}, {255, 237, 0},
{255, 238, 0}, {255, 239, 0}, {255, 240, 0}, {255, 241, 0}, {255, 241, 0},
{255, 242, 0}, {255, 243, 0}, {255, 244, 0}, {255, 245, 0}, {255, 246, 0},
{255, 247, 0}, {255, 248, 0}, {255, 249, 0}, {255, 250, 0}, {255, 250, 0},
{255, 251, 0}, {255, 252, 0}, {255, 253, 0}, {255, 254, 0}, {255, 255, 0},
}; 
//<===============================================0===============================================================>
//<==============================================time=============================================================>
//define variables for calculate the time for compute the heat didtridution in whatever(1D , 2D or 3D) dimensions division.
double start_tm,end_tm,tm_cmp;
timeval tim;
//declare a variable to store how many times jacobi used.
int jac_count = 0;
//<===============================================0===============================================================>
void Periodic(int value) {
	glutPostRedisplay();
	glutTimerFunc(refresh, Periodic, 0);
}

// (4x1) x (4 x 4)
void vecmat4(GLdouble *vec, GLdouble *mat, GLdouble *res){
	res[0] = vec[0]* mat[0] + vec[1]* mat[1] + vec[2]* mat[2] + vec[3]* mat[3];
	res[1] = vec[0]* mat[4] + vec[1]* mat[5] + vec[2]* mat[6] + vec[3]* mat[7];
	res[2] = vec[0]* mat[8] + vec[1]* mat[9] + vec[2]* mat[10] + vec[3]* mat[11];
	res[3] = vec[0]* mat[12] + vec[1]* mat[13] + vec[2]* mat[14] + vec[3]* mat[15];
}

// (4x4) x (4 x 4)
void matmat4(GLdouble *matA, GLdouble *matB, GLdouble *res){
	res[0] = matA[0]* matB[0] + matA[1]* matB[1] + matA[2]* matB[2] + matA[3]* matB[3];
	res[1] = matA[0]* matB[4] + matA[1]* matB[5] + matA[2]* matB[6] + matA[3]* matB[7];
	res[2] = matA[0]* matB[8] + matA[1]* matB[9] + matA[2]* matB[10] + matA[3]* matB[11];
	res[3] = matA[0]* matB[12] + matA[1]* matB[13] + matA[2]* matB[14] + matA[3]* matB[15];

	res[4] = matA[4]* matB[0] + matA[5]* matB[1] + matA[6]* matB[2] + matA[7]* matB[3];
	res[5] = matA[4]* matB[4] + matA[5]* matB[5] + matA[6]* matB[6] + matA[7]* matB[7];
	res[6] = matA[4]* matB[8] + matA[5]* matB[9] + matA[6]* matB[10] + matA[7]* matB[11];
	res[7] = matA[4]* matB[12] + matA[5]* matB[13] + matA[6]* matB[14] + matA[7]* matB[15];

	res[8] = matA[8]* matB[0] + matA[9]* matB[1] + matA[10]* matB[2] + matA[11]* matB[3];
	res[9] = matA[8]* matB[4] + matA[9]* matB[5] + matA[10]* matB[6] + matA[11]* matB[7];
	res[10] = matA[8]* matB[8] + matA[9]* matB[9] + matA[10]* matB[10] + matA[11]* matB[11];
	res[11] = matA[8]* matB[12] + matA[9]* matB[13] + matA[10]* matB[14] + matA[11]* matB[15];

	res[12] = matA[12]* matB[0] + matA[13]* matB[1] + matA[14]* matB[2] + matA[15]* matB[3];
	res[13] = matA[12]* matB[4] + matA[13]* matB[5] + matA[14]* matB[6] + matA[15]* matB[7];
	res[14] = matA[12]* matB[8] + matA[13]* matB[9] + matA[14]* matB[10] + matA[15]* matB[11];
	res[15] = matA[12]* matB[12] + matA[13]* matB[13] + matA[14]* matB[14] + matA[15]* matB[15];
}

double computeDistance(double x1, double y1, double z1, double x2, double y2, double z2){
	double xaxis = x2 - x1; 
	double yaxis = y2 - y1; 
	double zaxis = z2 - z1; 

	return sqrt(xaxis * xaxis + yaxis * yaxis + zaxis * zaxis); 
}

void initJacobi(){

	int i, j, k;
	before = (volatile double ***) malloc((cols + 2) * sizeof(double **));
	after = (volatile double ***) malloc((cols + 2) * sizeof(double **));

	for(i = 0; i < cols + 2; i++){
		before[i] = (volatile double **) malloc((rows + 2) * sizeof(double *));
		after[i] = (volatile double **) malloc((rows + 2) * sizeof(double *));
	}

	for(i = 0; i < cols + 2; i++){
		for(j = 0; j < rows + 2; j++){
			before[i][j] = (volatile double *) malloc((faces + 2) * sizeof(double));
			after[i][j] = (volatile double *) malloc((faces + 2) * sizeof(double));
		}
	}

	for(i = 0; i < cols + 2; i++){
		for(j = 0; j < rows + 2; j++){
			for(k = 0; k < faces + 2; k++){
				before[i][j][k] = ROOMTEMP;
				after[i][j][k] = ROOMTEMP;
			}
		}
	}
}

// Draw "atom" cube
void drawCube(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter, int index) {

	GLdouble vx = size; 
	GLdouble vy = size; 
	GLdouble vz = size; 

	glPushMatrix();
	glTranslatef(xcenter, ycenter, zcenter); 
	glBegin(GL_QUADS);
	// Front Face
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,vz);
	// Back Face
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,-vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,-vz);
	// Top Face
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,-vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,-vz);
	// Bottom Face
	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,-vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,-vz);
	// Left face
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(vx,vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(vx,-vy,vz);
	// Right face
	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,vz);
	
	glColor3ubv(palette[index]);
	glVertex3f(-vx,vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,-vz);

	glColor3ubv(palette[index]);
	glVertex3f(-vx,-vy,vz);
	glEnd();
	glPopMatrix();
}

void updateTemps(){
	int i, j, k;
	int xcenter, ycenter, zcenter;

	GLdouble dist;
	GLdouble temp;
	GLdouble exp = 2.0;
 	GLdouble fact = 0.0001;
	GLdouble pcoords[4];
	GLdouble scoords[4];
	GLdouble npcoords[4];
	GLdouble nscoords[4];

	scoords[0] = xsource;
	scoords[1] = ysource;
	scoords[2] = zsource;
	scoords[3] = 1.0;

	vecmat4(scoords, sourceRot, nscoords);

	for(i = 0, xcenter = -(cols + 2) + 1; i < cols + 2; i++, xcenter+=2){
		for(j = 0, ycenter = -(rows + 2) + 1; j < rows + 2; j++, ycenter+=2){
			for(k = 0, zcenter = -(faces + 2) + 1; k < faces + 2; k++, zcenter+=2){
				if(i == 0 || i == cols+1 || j == 0 || j == rows+1 || k == 0 || k == faces+1){

					pcoords[0] = xcenter*size;
					pcoords[1] = ycenter*size;
					pcoords[2] = zcenter*size;
					pcoords[3] = 0.0;

					vecmat4(pcoords, plateRot, npcoords);

					dist = computeDistance(nscoords[0], nscoords[1], nscoords[2], npcoords[0], npcoords[1], npcoords[2]);

					temp = TEMPRANGE / (fact * pow(dist,exp) + 1);
					before[i][j][k] = temp;
				}
			}
		}
	}
}

int moveSource(){
	int index = isrc++ % lensrc;

	xMove[SOURCE] = xsrc[index];
	yMove[SOURCE] = ysrc[index];
	zMove[SOURCE] = zsrc[index];
	//check when source is moving
	printf("Moving heat source to new position.\n");
	return isrc / lensrc;
}

void RenderScene(void) {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int i, j, k;
	
	//"Room"
	glPushMatrix();
	glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
	glutWireCube(240.0f);
	glPopMatrix();

	glPushMatrix();
		glRotatef(xRot[SOURCE], 1.0f, 0.0f, 0.0f);
		glRotatef(yRot[SOURCE], 0.0f, 1.0f, 0.0f);
		glRotatef(zRot[SOURCE], 0.0f, 0.0f, 1.0f);

		xsource = xMove[SOURCE];
		ysource = yMove[SOURCE];
		zsource = zMove[SOURCE];

	glTranslatef(xsource, ysource, zsource); 
	glColor3ub((GLubyte)255, (GLubyte)0, (GLubyte)0);
	glutSolidSphere(5.0f, 20, 20);

	glGetDoublev(GL_MODELVIEW_MATRIX, sourceRot);

	glPopMatrix();

	glPushMatrix();
		glRotatef(xRot[PLATE], 1.0f, 0.0f, 0.0f);
		glRotatef(yRot[PLATE], 0.0f, 1.0f, 0.0f);
		glRotatef(zRot[PLATE], 0.0f, 0.0f, 1.0f);

	// Map to palette color
	int index;
	//for checking
	//int a;
	glGetDoublev(GL_MODELVIEW_MATRIX, plateRot);

	GLdouble coords[4]; 

	for(i = 0, xcenter = -cols + 1; i < cols; i++, xcenter+=2){
		for(j = 0, ycenter = -rows + 1; j < rows; j++, ycenter+=2){
			for(k = 0, zcenter = -faces + 1; k < faces; k++, zcenter+=2){
				index = (int) rint(5*before[i+1][j+1][k+1]);

				coords[0] = xcenter*size; 
				coords[1] = ycenter*size; 
				coords[2] = zcenter*size; 
				coords[3] = 1;

				if(i == 0 || i == cols-1 || j == 0 || j == rows-1 || k == 0 || k == faces-1)
					drawCube(coords[0], coords[1], coords[2], index);
			}
		}
	}

	updateTemps();

	glPopMatrix();

	finish = FALSE;

	// In the case of multithreaded execution you will have to remove this call.
	// You will create a call to the function that starts the threads that executes
	// only once. Then the computation will take place on the background by the threads.
	//start the clock ...
	gettimeofday(&tim, NULL);
        start_tm=tim.tv_sec+(tim.tv_usec/1000000.0);
	jacobi();
	//increase times that jacobi method used.
	jac_count++;
	//stop the clock...
	gettimeofday(&tim, NULL);
        end_tm=tim.tv_sec+(tim.tv_usec/1000000.0);
	//find the accurate time.
	tm_cmp+=end_tm - start_tm;
	// Show the graphics
	glutSwapBuffers();
	//when jacobi finished and the heat source moved maximum repeats times,print the information about the experiment before closing.
	if(  (benchmark == TRUE) && (moveSource() == maxrepeats) )
		{
			printf("End of computation \n");
			printf("Informations :\n");
			printf("->Heat computated with serial jacobi algorithm.\n");
			printf("->Chosen repeats of computation: %d  and jacobi method used %d times.\n",maxrepeats,jac_count);
			//calculate the time that pthreads spend to compute heat distridution,
			//time result divided by maxrepeats give a average computation time. 
			printf( "Time results\n->Average computation time: %.10lf msec.\n",(tm_cmp*1000)/jac_count);
			printf("Terminating experiment!\n");
			printf("Goodbye!\n");		
			exit(0);		
			
		
		}
	
}

// Initialize rendering context
void SetupRC() {
	// background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
}

// Movement keys
void SpecialKeys(int key, int x, int y){
			
	int mod; 

	mod = glutGetModifiers(); 

	if(mod == GLUT_ACTIVE_CTRL) {
		if(key == GLUT_KEY_UP)
			xRot[object]-= 5.0f;

		if(key == GLUT_KEY_DOWN)
			xRot[object] += 5.0f;

		if(key == GLUT_KEY_LEFT)
			yRot[object] -= 5.0f;

		if(key == GLUT_KEY_RIGHT)
			yRot[object] += 5.0f;

		if(key == GLUT_KEY_PAGE_UP)
			zRot[object]-= 5.0f;

		if(key == GLUT_KEY_PAGE_DOWN)
			zRot[object] += 5.0f;

		if(key > 356.0f)
			xRot[object] = 0.0f;

		if(key < -1.0f)
			xRot[object] = 355.0f;

		if(key > 356.0f)
			yRot[object] = 0.0f;

		if(key < -1.0f)
			yRot[object] = 355.0f;

		if(key > 356.0f)
			zRot[object] = 0.0f;

		if(key < -1.0f)
			zRot[object] = 355.0f;
	}
	else {
		if(key == GLUT_KEY_UP)
			yMove[SOURCE]+= 25.0f;

		if(key == GLUT_KEY_DOWN)
			yMove[SOURCE]-= 25.0f;

		if(key == GLUT_KEY_LEFT)
			xMove[SOURCE]-= 25.0f;

		if(key == GLUT_KEY_RIGHT)
			xMove[SOURCE]+= 25.0f;

		if(key == GLUT_KEY_PAGE_UP)
			zMove[SOURCE]-= 25.0f;

		if(key == GLUT_KEY_PAGE_DOWN)
			zMove[SOURCE]+= 25.0f;
	}

	// Refresh the Window
	glutPostRedisplay();
}

void NormalKeys(unsigned char key, int x, int y){

	if(key == 'p'){
		object = PLATE;
		fprintf(stderr, "Changed to object = %d (key = %c)\n", object, key);
	}

	if(key == 's'){
		object = SOURCE;
		fprintf(stderr, "Changed to object = %d (key = %c)\n", object, key);
	}

	if(key == 'i'){
		benchmark = FALSE;
		fprintf(stderr, "Changed to mode = %d (key = %c)\n", benchmark, key);
	}

	if(key == 'b'){
		benchmark = TRUE;
		isrc = 0;
		fprintf(stderr, "Changed to mode = %d (key = %c)\n", benchmark, key);
	}
}

void ChangeSize(int w, int h) {
	GLdouble fAspect;

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (GLdouble)w / (GLdouble)h;
	gluPerspective(35.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -400.0f);
}

void jacobi(){
	int i, j, k;
	double fact = 1.0;
	double diff = 0.0;

	int iterations = 0;

	fact = fact / 6;
	while(finish == FALSE){
		diff = 0.0;		//find the new temperature and store it in the heat array(after [][][]).
		for(i = 1; i <= cols; i++){
			for(j = 1; j <= rows; j++){
				for(k = 1; k <= faces; k++){
					after[i][j][k] = fact * (before[i - 1][j][k] + before[i + 1][j][k] + before[i][j - 1][k] + before[i][j + 1][k] + before[i][j][k-1] + before[i][j][k+1]);

					if(fabs(after[i][j][k] - before[i][j][k]) > diff)
						diff = fabs(after[i][j][k] - before[i][j][k]);
				}
			}
		}
		//update heat array(before [][][]).
		for(i = 1; i <= cols; i++){
			for(j = 1; j <= rows; j++){
				for(k = 1; k <= faces; k++){
					before[i][j][k] = after[i][j][k];
				}
			}
		}

		iterations++;

		if(diff <= EPSILON){
			printf("Heat distribution stabilised after %d iterations (diff = %.24f)\n", iterations, diff);
			finish = TRUE;
			//return 0;
			/*if(benchmark == TRUE){
				if(moveSource() == maxrepeats)
					exit(0);
			}*/
		}//close if examinid diff.
	}//close while.
}//close serial jacobi.

void usage(const char *prog){
	fprintf(stderr, "Usage %s [-b|-i]\n", prog);
	fprintf(stderr, "\t-b:\t Benchmark mode\n");
	fprintf(stderr, "\t-i:\t Interactive mode\n");
	fprintf(stderr, "\t-l=val:\t Number of benchmark iterations\n");
}

int main(int argc, char** argv)
{
	int ch; 

	opterr = 0;
	while ((ch = getopt (argc, argv, "bir:")) != -1){
		switch (ch)
		{
			case 'b':
				benchmark = TRUE;
				break;
			case 'i':
				benchmark = FALSE;
				break;
			case 'r':
				maxrepeats = atoi(optarg);
				printf("Thanks,You chose %d repeats of computation of heat distridution.\n ",maxrepeats);
				break;
			case '?':
				usage(argv[0]);
				exit(0);
			default:
				usage(argv[0]);
				exit(0);
		}
	}
	//initialize time variable with 0
	printf("Computing heat distridution with serial algorithm.\n");
	tm_cmp=0.0;
	initJacobi();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024,768);
	glutCreateWindow("Heat Distribution");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(NormalKeys);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(refresh, Periodic, 0);
	SetupRC();

	glutMainLoop();
	//printf("Hello after glutMainLOop. \n");
	return 0;
}
