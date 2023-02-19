//OGLFPS.cpp
void displayFunc();
void idleFunc();
void gameFunc(int);
void keyboardFunc(GLubyte,GLint,GLint);
void motionFunc(GLint,GLint);
void mouseFunc(GLint,GLint,GLint,GLint);
void passiveMotionFunc(GLint,GLint);
void reshapeFunc(GLint,GLint);



//function.hpp
void beingInitialization();
void earlyUpdate();
void glewInitialization();
void glutInitialization();
void initialization();
void keyboard();
void lateUpdate();
void mouse();
void update();

//print.hpp
void constvPrint();
void frameTimePrint();
template<typename TN>
void matrixPrint(int,int,const TN* const);
void print();
void staticvPrint();
template<typename TN>
void vectorPrint(int,const TN* const);
template<typename TN>
void vectorPrint_index(const vector<int>&,const vector<TN>&);

//universal.hpp
bool beingIsOver(Being&);
bool bulletIsOver(const Bullet&);
bool groupIsOver(const Group&);
bool rayIsOver(const Ray&);
int collideBeing_heal(MeshCube&,MeshCube* const);
int collidePlayer_damage(const MeshCube&);
void addGroup(const Point3d* const);
void countDown(int);
void displayScene();
void drawCube(int,const double* const);
void drawGG();
void drawGround(const double* const);
void drawRectangular(int,const double* const);
void drawSign();
void drawWall(const double* const);
void drawWin();
void gameOver(int);
void gamePause(int);
void generateGroup();
void generatePivSph(Point3d* const);
void globalRGB();
void isGameOver();
void polarRight(const double* const,double* const);
void polar_rectangularRight(const double* const,double* const);
void randomCoor2D(double* const);
void randomSceneArea(int* const);
void randomSceneCoor2D(double* const);
void spherical_rectangularRight(const double* const,double* const);
void spherical_rectangularUp(const double* const,double* const);
void sphericalRight(const double* const,double* const);
void sphericalUp(const double* const,double* const);
void toggleGameMode();
