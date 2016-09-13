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
//the next jacodi function 'uses' openmp feature static scheduling for threads.
void jacobi_st_schd();//the next jacodi function 'uses' openmp feature guided scheduling for threads.
void jacobi_gd_schd();
