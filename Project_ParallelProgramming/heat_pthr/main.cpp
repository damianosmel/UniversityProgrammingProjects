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
//change it to make reasonable results
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

//<=============================================0=============================================================>
//num_thrd corresponds to how many threads user choose.
int num_thrd;
//<===============================Declaration of array=======================================================>
//pointers at start of the array used pthreads.
pthread_t *threads;
//<===============================Declaration of Barriers======================================================>
// Barrier variable after which threads update the array with new temparature.
//A second barrier to examine if the temperature of all table(plaka) is precisely computed(diff<EPSILON).
//And a third barrier to examine if all pthreads print their useful informations (barr_to_print).
pthread_barrier_t barr_to_update;
pthread_barrier_t barr_to_examine;
pthread_barrier_t barr_to_print;
//<===============================Declaration of Mutex=======================================================>
//A mutex to have only one thread increase the variable iterations
//secondly to have again only one thread the ability to access the variable good_conversion
pthread_mutex_t rc2;
//pthread_mutex_t rc3;
//<===========================================X-D==============================================================>
//flag to see if all part's temperature of the table have been successfully computed (diff <EPSILON private diff for each thread).
//i corresponds to a part of x axis or x-y axis or x-y-z axis depending on the chosen dimensional computation of heat.
//this variable is used in 1,2 and 3 dimensions edition of jacobi.
int good_convertion=0;
//<===========================================time=============================================================>
//define variables for calculate the time for compute the heat didtridution in whatever(1D , 2D or 3D) dimensions division.
double start_tm,end_tm,tm_cmp;
//define a timeval variable to store information given by gettimeofday(&timeval tm,NULL)
timeval tim;
//<===========================================1-D==============================================================>
//for computation in one dimension.
//declaring a variable to store the part of  x-axis to computation.
//example thrd_part_x_x stands for computational part of x-axis for a thread.
//declaring a variable to store how many dimensions the user chose .
int dim_choice;
int thrd_part_x;

//<============================================2-D=============================================================>
//for computation in two dimensions.
//declaring two variables to store the part of an axis to computation.
//example part_k_2d stands for computational part of k-axis for a thread.
int part_x_2d;
int part_y_2d;
//declaring one variable to know how many ptrheads u have in each axis. 
//warning:user have to give a number of threads which will have an integer square root (eqaul number of pthreads in each axis).
int num_pthr_axis;

//<=============================================3-D=============================================================>
//for computation in three dimensions.
//declaring one variable to know how many ptrheads u have in each axis. 
//warning:user have to give a number of threads which will have an integer cube root (eqaul number of pthreads in each axis).
int num_pthr_axis1;
//declaring variable to know how many pthreads you have in 2 dimension (having z cordinate constant).
int num_pthr_2axis;
//declaring three variables to store the part of an axis to computation.
//example part_k_3d stands for computational part of k-axis for a thread.
int part_x_3d;
int part_y_3d;
int part_z_3d;
//declare a variable that will be returned.
void * return_value;
//declare a counter that stores how many times jacobi was used.
int jac_count=0;
//<==============================================0===============================================================>

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
	printf("Moving heat source to new position.\n");
	//check that source is moving.
	//printf("returned value: %d. \n",isrc/lensrc);
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
        //for multithreading
	int thr_count;

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
	

	//jacobi();
	
		
		//start the clock ...
		gettimeofday(&tim, NULL);
        	start_tm=tim.tv_sec+(tim.tv_usec/1000000.0);
		if( dim_choice == 1 )
		{	
			//<==================================1-D====================================>
			//Calling pthreads to execute sharing only  x-axis.
			//printf("Calling pthreads to execute sharing only x-axis .\n");
			for(thr_count=0;thr_count<=num_thrd-1;thr_count++)
			{	
				if(pthread_create(&threads[thr_count],NULL,&jacobi_1dim,(void*)thr_count))
				{
					printf("Could not create thread %d.Computing in x-axis only\n", thr_count);
           	        		exit(-1);
				}//close inner if.
	
			}//close for.	
			//increase used times of jacobi.
			jac_count++;
		}//close if-statement for one dimensional division. 
		
		if( dim_choice == 2 )
		{		
			//<===============================2-D=====================================>
			//Calling pthreads to execute sharing x-axis and y-axis.
			//printf("Calling pthreads to execute sharing x-axis and y-axis. \n");
			for(thr_count=0;thr_count<=num_thrd-1;thr_count++)
			{	
				if(pthread_create(&threads[thr_count],NULL,&jacobi_2dim,(void*)thr_count))
				{
					printf("Could not create thread %d.Computing in x and y axis .\n", thr_count);
           	        		exit(-1);
				}//close inner if.	
		
			}//close for
			//increase used times of jacobi.
			//printf("threads created.\n");
			jac_count++;
		}//close if-statement for two dimensional division.

		if( dim_choice == 3 )
		{
			//<===============================3-D=======================================>
			//Calling pthreads to execute sharing x-axis,y-axis and z-axis.
			//printf("Calling pthreads to execute sharing x-axis,y-axis and z-axis. \n");
			for(thr_count=0;thr_count<=num_thrd-1;thr_count++)
			{	
				if(pthread_create(&threads[thr_count],NULL,&jacobi_3dim,(void*)thr_count))
				{
					printf("Could not create thread %d.Computing in x,y and z axis .\n", thr_count);
           	        		exit(-1);
				}	


			}//close for 
			//increase used times of jacobi.
			jac_count++;
		}//close if-statement for three dimensional division.
		
		//make RenderScene wait all threads defore "she" terminates.
		//printf("before thread join\n.");
		for(i=0;i<=num_thrd-1;i++)
		{	
			pthread_join(threads[i],NULL);
		}
		//stop the clock...
		gettimeofday(&tim, NULL);
        	end_tm=tim.tv_sec+(tim.tv_usec/1000000.0);
		//find the accurate time
		tm_cmp+=end_tm - start_tm;
		// Show the graphics
		glutSwapBuffers();	
		/*when posix threads computed the heat distridution as long as benchmark mode is selected ,look the how many times heat source moved.if moved maxrepeats times print some useful informations for the experiment and close program.*/ 
		if( (benchmark == TRUE) && (moveSource() == maxrepeats ) )
		{
		printf("End of computation. \n");
		printf("Informations :\n");
		printf("->Chosen number of pthreads: %d\tUsing %d-Dimensional division,\n->Chosen repeats of computation: %d  and jacobi method used %d times.\n"
		,num_thrd,dim_choice,maxrepeats,jac_count);
		
		//calculate the average time that pthreads spend to compute heat distridution,
		//time result divided by maxrepeats give a average computation time. 
		printf( "Time results\n->Average computation time: %.10lf msec.\n",(tm_cmp*1000)/jac_count);
		//exiting without living waste ...free allocated memory for threads[] .
		printf("Terminating experiment!\n");
		printf("Clearing used memory.\n");
		printf("Please Wait...\n");
		printf("Free allocated memory for  threads[] array.\n");
		free(threads);
		printf("Goodbye!\n");		
		exit(0);		
		
		}//close if-finish statement. 

}//close RenderScene().

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

//jacobi made for computing in x-axis.
void * jacobi_1dim(void *pid){
	int i, j, k;
	
	//change the void pointer to int integer
	//thread's id  is the part  of it's  contribution to the jacobi function.
	long int part=(long int)pid;
	double fact=1.0;
	double diff=0.0;
	int my_iterations=0;
	fact = fact / 6;
	while(finish == FALSE){
		diff = 0.0;
		//all threads help to compute the heat array (after[][][]) with the jacobi method in one dimension.
		for(i = 1+part*thrd_part_x; i <= (part+1)*thrd_part_x && i<= cols; i++){
			for(j = 1; j <= rows; j++){
				for(k = 1; k <= faces; k++){
					after[i][j][k] = fact * (before[i - 1][j][k] + before[i + 1][j][k] + before[i][j - 1][k] + 
					before[i][j + 1][k] + before[i][j][k-1] + before[i][j][k+1]);

					if(fabs(after[i][j][k] - before[i][j][k]) > diff)
						diff = fabs(after[i][j][k] - before[i][j][k]);
				}//close for faces.
			}//close for rows.
		}//close for cols.


		//all threads have to come here in order to go on and update the array of new (after) temperature.
		// Synchronization point
		good_convertion=0;
		//printf("hello from %ld in good_conversion=0\n",part);		
     		int rc = pthread_barrier_wait(&barr_to_update);
   		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to update.\n");
        		exit(-1);
    		}
		//all threads help to update the heat array (before[][][]).		
		for(i = 1+part*thrd_part_x; i <=(part+1)*thrd_part_x && i<=cols; i++){
			for(j = 1; j <= rows; j++){
				for(k = 1; k <= faces; k++){
					before[i][j][k] = after[i][j][k];
				}//close for faces.
			}//close for rows.
		}//close for cols.
		/*lock this critical section so as to one thread each time can increase the variable iterations and have only one thread increase the variable if it's diff<=EPSILON.*/
		pthread_mutex_lock(&rc2);		
		my_iterations++;
		
		if( diff <= EPSILON )
		{
			good_convertion++;
		}	
		//unlock the section to let the other threads to access this section.	
		pthread_mutex_unlock(&rc2);


		//second barrier to have all threads examine convertion.
		int rc1 = pthread_barrier_wait(&barr_to_examine);
   		if(rc1 != 0 && rc1 != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to examine.\n");
        		exit(-1);
    		}
		//examine good_convertion,if it is equal to number of threads you finish! 
		if( good_convertion == num_thrd ){
			/*each thread prints its' cordinates in x axis only and its' difference of heat and the maximum iterarions which a thread made to have the heat stabilized in its' correspondant part. */
			printf("In %ld part of x-axis:  Heat distribution stabilised after %d iterations (diff = %.24f)\n",part,my_iterations,diff);
			//all threads have to print the previous message in order to move on all together using next barrier (barr_to_print).  
			int rc4 = pthread_barrier_wait(&barr_to_print);
   			if(rc4 != 0 && rc4 != PTHREAD_BARRIER_SERIAL_THREAD)
     			{
        			printf("Could not wait on barrier to print.\n");
        			exit(-1);
    			}//close if.
			finish = TRUE;
			if(benchmark == TRUE){
					//in phtread version u have pthread_exit(NULL) which is eqaul to exit(0) in sequential version.
					//exit(0);
					pthread_exit(NULL);
			}//close  if examining benchmark.
		}//close if examining good convertion.
	//printf("closing while\n");	
	
	}//close while
	return(return_value);
	
}//close * jacobi_1dim.

//jacobi made for computing in x-axis and y-axis.
void * jacobi_2dim(void *pid){
	int i, j, k;
	
	//change the void pointer to int integer
	//thread's id  is the part  of it's  contribution to the jacobi function.
	long int temp_part=(long int)pid;
	int part_x,part_y;
	//find from pid the coordinates of pthread in x and y axis.
	part_y=(int)temp_part/num_pthr_axis; 
	part_x=(int)temp_part%num_pthr_axis;
	//printf("Hello there,from jacobi 2dim .\n");
	double fact=1.0;
	double diff=0.0;
	int my_iterations=0;
	//find the limits of computation for each thread.
	//int start=part_x*part_x_2d;
	//int end=part_
	fact = fact / 6;
	while(finish == FALSE){
		diff = 0.0;
		//all threads help to compute the heat array (after[][][]) with the jacobi method in two dimensions .
		for(i =1+part_x*part_x_2d; i <= (part_x+1)*part_x_2d && i<=cols; i++){
			for(j =1+part_y*part_y_2d; j <= (part_y+1)*part_y_2d && j<=rows; j++){
				for(k = 1; k <= faces; k++){
					after[i][j][k] = fact * (before[i - 1][j][k] + before[i + 1][j][k] + before[i][j - 1][k] +
					 before[i][j + 1][k] + before[i][j][k-1] + before[i][j][k+1]);

					if(fabs( after[i][j][k] - before[i][j][k]) > diff )
						diff = fabs(after[i][j][k] - before[i][j][k]);
				}//close for faces.
			}//close for rows.
		}//close for cols.
		

	//all threads have to come here in order to go on and update the array of new (after) temparature.
	// Synchronization point
		good_convertion=0;
     		int rc = pthread_barrier_wait(&barr_to_update);
   		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to update.\n");
        		exit(-1);
    		}
		//all threads help to update the heat array (before[][][]).	
		for(i = 1+part_x*part_x_2d; i <= (part_x+1)*part_x_2d && i<=cols; i++){
			for(j = 1+part_y*part_y_2d; j <= (part_y+1)*part_y_2d && j<=rows ; j++){
				for(k = 1; k <= faces; k++){
					before[i][j][k] = after[i][j][k];
				}//close for faces.
			}//close for rows.
		}//close for cols.
		/*lock this critical section so as to one thread each time can increase the variable iterations and have only one thread increase the variable if it's 	diff <=EPSILON. */
		pthread_mutex_lock(&rc2);		
		my_iterations++;
		if( diff <= EPSILON )
		{
			good_convertion++;
		}	
		//unlock the section to let the other threads to access this section.		
		pthread_mutex_unlock(&rc2);


		//second barrier to have all threads examine convertion.
		int rc1 = pthread_barrier_wait(&barr_to_examine);
   		if(rc1 != 0 && rc1 != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to examine.\n");
        		exit(-1);
    		}
		//examine good_convertion,if it is equal to number of threads u finish! 
		if( good_convertion == num_thrd ){
			//part_y stands for cordinates in y-axis.
			//part_x stands for cordinates in x-axis.
			/*each thread prints its' cordinates in x,y axis and its' difference of heat and the maximum iterarions which a thread made to have the heat stabilized in its' correspondant part. */
			printf("In (%d,%d) part of x and y axis:  Heat distribution stabilised after %d iterations (diff=%.24f)	\n",part_x,part_y,my_iterations,diff);
			//all threads have to print the previous message in order to move on all together using the next barrier (barr_to_print).  
			int rc4 = pthread_barrier_wait(&barr_to_print);
   			if(rc4 != 0 && rc4 != PTHREAD_BARRIER_SERIAL_THREAD)
     			{
        			printf("Could not wait on barrier to print.\n");
        			exit(-1);
    			}//close if.
			finish = TRUE;
			if(benchmark == TRUE){
					//in phtread version u have pthread_exit(NULL) which is eqaul to exit(0) in sequential version.
					//exit(0);
					pthread_exit(NULL);
			}//close  if examining benchmark.
		}//close if examining good convertion.	
	}//close while.
	return(return_value);
}//close *jacobi_2dim

//jacobi made for computing in x-axis,y-axis and z-axis.
void * jacobi_3dim(void *pid){
	int i, j, k;
	
	//change the void pointer to int integer,
	//thread's id  is the part  of it's  contribution to the jacobi function.
	long int temp_part=(long int)pid;
	//store the module of division bewtween temp_part and num_pthr_2axis.	
	int mod_part;
	int my_iterations=0;
	int part_x,part_y,part_z;
	//find from pid the coordinates of pthread in x,y and z axis.
	//find the z cordinate dividing pid with number of ptreads used in x and y axis(2 axis).
	part_z=(int)temp_part/num_pthr_2axis; 
	mod_part=(int)temp_part%num_pthr_2axis;
	/*the module of previous division is divided by number of threads used in x or y axis(1 axis).in order to find the y cordinate and the x cordinate.*/
	part_y=mod_part/num_pthr_axis1;
	part_x=mod_part%num_pthr_axis1;
	
	double fact=1.0;
	double diff=0.0;
	fact = fact / 6;
	while(finish == FALSE){
		diff = 0.0;
		//all threads help to compute heat array(after[][][]) with the jacobi method in three dimensions .
		for(i = 1+part_x*part_x_3d; ((i <= (part_x+1)*part_x_3d) && (i<=cols)); i++){
		
			for(j = 1+part_y*part_y_3d; ((j <= (part_y+1)*part_y_3d) && (j<=rows)); j++){
				
				for(k = 1+part_z*part_z_3d; ((k <= (part_z+1)*part_z_3d )&&( k<=faces)); k++){
					after[i][j][k] = fact * (before[i - 1][j][k] + before[i + 1][j][k] + before[i][j - 1][k] +
					 before[i][j + 1][k] + before[i][j][k-1] + before[i][j][k+1]);

					if( fabs( after[i][j][k] - before[i][j][k] ) > diff )
					{

					
						diff = fabs(after[i][j][k] - before[i][j][k]);
					}//close if.
				
				}//close for faces.
			}//close for rows.
		}//close for cols.
		//all threads have to come here in order to go on and update the array of new (after) temparature.
		// Synchronization point
		good_convertion=0;
     		int rc = pthread_barrier_wait(&barr_to_update);
   		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to update.\n");
        		exit(-1);
    		}
		
		
		//all threads help to update the heat array (before[][][]).	
		for(i = 1+part_x*part_x_3d; ((i <= (part_x+1)*part_x_3d )&&( i<=cols)); i++){
			
			for(j = 1+part_y*part_y_3d;(( j <= (part_y+1)*part_y_3d )&&( j<=rows )); j++){
				
				for(k = 1+part_z*part_z_3d;(( k <= (part_z+1)*part_z_3d )&&( k<=faces )); k++){
					before[i][j][k] = after[i][j][k];
				}//close for faces.
			}//close for rows.
		}//close for cols.

		//lock this critical section so as to one thread each time can increase the variable iterations and have only one thread increase the variable if it's diff <=EPSILON.
		pthread_mutex_lock(&rc2);		
		
		my_iterations++;
		if(diff <= EPSILON)
		{
			good_convertion++;
		}	
		//unlock this critical section.		
		pthread_mutex_unlock(&rc2);

		//second barrier to have all threads examine convertion.
		int rc1 = pthread_barrier_wait(&barr_to_examine);
   		if(rc1 != 0 && rc1 != PTHREAD_BARRIER_SERIAL_THREAD)
     		{
        		printf("Could not wait on barrier to examine.\n");
        		exit(-1);
    		}
		//examine good_convertion,if it is equal to number of threads u finish! 
				
		if( good_convertion == num_thrd )
		{
			//part_z stands for cordinates in z-axis.
			//part_y stands for cordinates in y-axis.
			//part_x stands for cordinates in x-axis.
			/*each thread prints its' cordinates in x,y and z axis and its' difference of heat and the maximum iterarions which a thread made to have the heat stabilized in its' correspondant part. */
			printf("In (%d,%d,%d) part of x,y,z-axis:  Heat distribution stabilised after %d iterations (diff = %.24f)\n",part_x,part_y,part_z,my_iterations,diff);
			//all threads have to print the previous message in order to move on all together using the next barrier (barr_to_print).  
			int rc4 = pthread_barrier_wait(&barr_to_print);
   			if(rc4 != 0 && rc4 != PTHREAD_BARRIER_SERIAL_THREAD)
     			 	{
        				printf("Could not wait on barrier to print.\n");
        				exit(-1);
    				}
			finish = TRUE;
			if(benchmark == TRUE){
					//in phtread version u have pthread_exit(NULL) which is eqaul to exit(0) in sequential version.					
					//exit(0);
					pthread_exit(NULL);
				
			}//close  if examining benchmark.
		}//close if examining good convertion.
	}//close while.
	return(return_value);
}//close *jacobi_3dim.

void usage(const char *prog){
	fprintf(stderr, "Usage %s [-b|-i]\n", prog);
	fprintf(stderr, "\t-b:\t Benchmark mode\n");
	fprintf(stderr, "\t-i:\t Interactive mode\n");
	fprintf(stderr, "\t-l=val:\t Number of benchmark iterations\n");
}

int main(int argc, char** argv)
{
	int ch; 
	//int i;
	//have this variable so if you have to proceed to glutMainloop depending on the user's choise.
	int miss;
	miss=0;
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
				printf("Thanks,You chose %d repeats of computation of heat distridution. \n ",maxrepeats);
				break;
			case '?':
				usage(argv[0]);
				exit(0);
			default:
				usage(argv[0]);
				exit(0);
		}
	}
//<============================0=============================>	
	printf("Computing heat distridution with pthreads' help.\n");
	printf("\nPlease give number of threads helping the computation.\n");
	 scanf("%d",&num_thrd);
	printf("\nThanks,You chose :\t%d\n",num_thrd);
	
/*<==================== array of threads=====================>*/
	threads=(pthread_t *)malloc( num_thrd * sizeof(pthread_t) );	
	if( threads == NULL )
	{	printf("Sorry,could not allocate memory for threads\n"); 
	}
	else{
		
			//Initialization of mutexes and barriers.	
			// Barrier initialization for updating (rc).
    			if( pthread_barrier_init(&barr_to_update, NULL,num_thrd) )
    			{
        			printf("Could not create the barrier to update.\n");
        			return -1;
    			}
			//Barrier initialization for examining good_conversion (rc1).
			if( pthread_barrier_init(&barr_to_examine, NULL,num_thrd) )
    			{
        			printf("Could not create the barrier to examine.\n");
        			return -1;
    			}
			//Barrier initialization for printing informations (rc4).
			if( pthread_barrier_init(&barr_to_print, NULL,num_thrd) )
    			{
        			printf("Could not create the barrier to print.\n");
        			return -1;
    			}
			// Initialize the mutex for having only one pthread increase the iterations and if diff<EPSILON increase also good_conversion (rc2).
    			if(pthread_mutex_init(&rc2, NULL))
    			{
        			printf("Unable to initialize a mutex for iterations.\n");
        			return -1;
    			}
			// Initialize the mutex for having only one pthread increase the good conversion (rc3).
    			/*if(pthread_mutex_init(&rc3, NULL))
    			{
        			printf("Unable to initialize a mutex for increasing conversion.\n");
        			return -1;
    			}*/
			

		do{
			printf("Please,what dimensional division do you want ? \n");
			printf("Choices : \n"); 
			printf("1->one dimensional division\t 2->two dimensional division\n3->three dimensional division\t");
			printf("-1->terminate experiment.\n");
			 scanf("%d",&dim_choice);
		switch( dim_choice ){
			
			case 1:
				miss=0;
				//threads share only the x-axis.
				//found the divided computional part for each thread in x-axis. 
				thrd_part_x=cols/num_thrd;
				//clear the time variable for one dimension computation.
				tm_cmp=0.0;
				
				printf("Please wait, computation in one dimension (x) axis starting. \n");
				break;
			case 2:		
				miss=0;	
				//threads share x-axis and y-axis.
				//find the number of threads in each axis.
				num_pthr_axis=(int)sqrt((double)num_thrd);
				
				//calculate the computional part in each axis(x and y) for a thread.
				part_x_2d=cols/num_pthr_axis;
				part_y_2d=rows/num_pthr_axis;
				//clear the time variable for two dimensions computation.
				tm_cmp=0.0;
				printf("Please wait, computation in two dimensions (x-y) axis starting. \n");
				break;		
			case 3:
				miss=0;
				//threads share x-axis,y-axis and z-axis.
				//find the number of threads in each axis,with cubic root's help.		
				num_pthr_axis1=(int)cbrt((double)num_thrd);
				
				//find how many threads are in 2 dimensions(x-y axis).
				num_pthr_2axis=num_pthr_axis1 * num_pthr_axis1;
				//num_pthr_2axis=num_thrd/num_pthr_axis1;
				//calculate the computional part in each axis(x,y and z) for a thread.
				part_x_3d=cols/num_pthr_axis1;
				part_y_3d=rows/num_pthr_axis1;
				part_z_3d=faces/num_pthr_axis1;
				//clear the time variable for three dimensions computation.
				tm_cmp=0.0;
				printf("Please wait, computation in three dimensions (x-y-z) axis starting. \n");
				break;
			case -1:
				miss=1;
				printf("Terminating experiment!\n");
				printf("Clearing used memory.\n");
				printf("Please Wait...\n");
					
				break;
			default :
				miss=1;
				printf("Please give a value of the given choices. \n");
		}//close switch.
		//only if user chose correctly dimensional division proceed to glutMainLoop.
		if(!miss){
			
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
			
		}//close if
		
		
		
		}while(dim_choice != -1);
		//free allocated memory for the array of threads.
		//when user wants to live experiment and memory for threads array was allocated at first 
		printf("Free allocated memory for  threads[] array.\n");
		free(threads);
	}//close else
	printf("Goodbye!\n");
	return 0;
}
