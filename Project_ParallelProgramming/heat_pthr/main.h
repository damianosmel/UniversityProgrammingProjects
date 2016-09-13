void Idle();
void Periodic(int value);
void vecmat4(GLdouble *vec, GLdouble *mat, GLdouble *res);
void matmat4(GLdouble *matA, GLdouble *matB, GLdouble *res);
double computeDistance(double x1, double y1, double z1, double x2, double y2, double z2);
void initJacobi();
void drawCube(GLdouble xcenter, GLdouble ycenter, GLdouble zcenter, int index);
void updateTemps();
int moveSource();
void RenderScene(void);
void SetupRC();
void SpecialKeys(int key, int x, int y);
void NormalKeys(unsigned char key, int x, int y);
void ChangeSize(int w, int h) ;
void usage(const char *prog);
//jacobi for one dimension with pthreads' help.
void *jacobi_1dim(void *arg);
//jacobi for two dimensions with pthreads' help.
void *jacobi_2dim(void *arg);
//jacobi for three dimensions with pthreads'help.
void *jacobi_3dim(void *arg);
