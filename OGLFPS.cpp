#include<conio.h>
#include<GL/glut.h>
#include<limits.h>
#include<math.h>
#include<time.h>
#include<windows.h>

#include<algorithm>
#include<functional>
#include<iostream>
#include<list>
#include<map>
#include<vector>

using namespace std;

//CAP
#include"CAP/win.h"

#include"CAP/GLT.hpp"

#include"CAP/GJK.hpp"
#include"CAP/Liang_Barsky.hpp"
#include"CAP/OGLT.hpp"

using namespace CAP;
using namespace CAP::OGL;

//project
#include"class.hpp"

#include"declare.hpp"

#include"global.hpp"

#include"functor.hpp"

#include"calculation.hpp"
#include"function.hpp"
#include"print.hpp"
#include"universal.hpp"



void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//globalRGB();
	displayScene();
	/*
	for_each(beingList().begin(),beingList().end(),DisplayBeing());
	*/
	for_each(beingAList().begin(),beingAList().end(),DisplayBeing());
	for_each(beingBList().begin(),beingBList().end(),DisplayBeing());
	for_each(beingGList().begin(),beingGList().end(),DisplayBeing());
	for_each(beingRList().begin(),beingRList().end(),DisplayBeing());
	for_each(bulletList().begin(),bulletList().end(),DisplayBullet());
	for_each(rayList().begin(),rayList().end(),DisplayRay());
	if(gameMode==TOP_DOWN_SHOOTER){player().display();}
	
	glLoadIdentity();
	gluOrtho2D(0,canvasWidth,0,canvasHeight);
	player().displayFlashBar();
	player().displayHPBar();
	player().displayLaserBar();
	if(gameMode==FPS){drawSign();}
	
	if(goodGame==1){drawWin();}
	else if (goodGame==2){drawGG();}
}
void idleFunc(){}
void gameFunc(int value)
{
	if(0< player().getHP()){earlyUpdate();}
	update();
	lateUpdate();

	isGameOver();
	glutPostRedisplay();
	
	frameOver=GetTickCount();
	deltaTime=constv().frameTime*((frameOver-frameStart)/constv().frameTime+1);
	frameIdle=constv().frameTime-((frameOver-frameStart)%constv().frameTime);
	//cout<<"frameIdle  "<<frameIdle<<endl;
	CAP_pause_spin(frameIdle);
	frameStart=GetTickCount();
	glutSwapBuffers();

	while(gameState){}

	glutTimerFunc(0,gameFunc,0);
}
void keyboardFunc(GLubyte key,GLint xMouse,GLint yMouse)
{
	switch(key)
	{
		case 'C':
		case 'c':toggleGameMode();break;
		case 'G':
		case 'g':player().setHP(player().getHP()-1);break;
		case 'H':
		case 'h':player().setHP(player().getHP()+1);break;
		case 'K':
		case 'k':generateGroup();break;
		case 'L':
		case 'l':print();break;
		case 'M':
		case 'm':player().setHP(INT_MAX);break;
		case 'N':
		case 'n':player().setAutomatic(!player().getAutomatic());break;
		case 'O':
		case 'o':exit(0);break;
		case 'P':
		case 'p':glutTimerFunc(0,gamePause,0);break;
		default:break;
	}
}
void motionFunc(GLint xMouse,GLint yMouse){}
void mouseFunc(GLint button,GLint action,GLint xMouse,GLint yMouse){}
void passiveMotionFunc(GLint xMouse,GLint yMouse){}
void reshapeFunc(GLint newWidth,GLint newHeight)
{
	glutInitWindowSize(canvasWidth,canvasHeight);
	/*
	canvasHeight=newHeight;
	canvasWidth=newWidth;
	glViewport(0,0,canvasWidth,canvasHeight);
	*/
}

int main(int argc,char** argv)
{
	initialization();
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(windowPX,windowPY);
	glutInitWindowSize(canvasWidth,canvasHeight);
	glutCreateWindow("OGLFPS");
	glutInitialization();
	
	//glewInitialization();
	
	glutDisplayFunc(displayFunc);
	//glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	//glutMotionFunc(motionFunc);
	//glutMouseFunc(mouseFunc);
	//glutPassiveMotionFunc(passiveMotionFunc);
	//glutReshapeFunc(reshapeFunc);
	
	glutTimerFunc(0,gamePause,0);
	glutTimerFunc(0,gameFunc,0);
	glutMainLoop();
	
	return 0;
}
