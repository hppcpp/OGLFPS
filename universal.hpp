bool beingIsOver(Being& being)
{
	if(being.getHP()<=0 && Being::corpseSpell<=GetTickCount()-being.getStopwatch())
	{
		being.setGroup()->beingPtr[being.RGBA1()]=0;
		return true;
	}
	return false;
}
bool bulletIsOver(const Bullet& bullet)
{
	if(bullet.getSpell()<=GetTickCount()-bullet.getStopwatch()){return true;}
	return false;
}
bool groupIsOver(const Group& group)
{
	for(int i=0;i< 4;i++)
	{
		if(group.beingPtr[i]){return false;}
	}
	return true;
}
bool rayIsOver(const Ray& ray)
{
	if(Ray::spell<=GetTickCount()-ray.getStart()){return true;}
	return false;
}
int collideBeing_heal(MeshCube& cube1,MeshCube* const cube2)
{
	if(Being::HPfull<=cube1.getHP() || cube2->getHP()<=0){return 0;}

	int collision=0;
	vector<Point2d> rect1,rect2;

	cube1.getGlobalVertex2_z(rect1);
	cube2->getGlobalVertex2_z(rect2);
	collision=GJK2D(rect1,rect2);
	if(collision)
	{
		cube1.setHP(cube1.getHP()+cube2->getHP());
		cube2->setHP(0);
		cube2->corpse();
	}

	return collision;
}
int collidePlayer_damage(const MeshCube& cube)
{
	int collision=0;
	vector<Point2d> plaRect,rect;

	player().getGlobalVertex2_z(plaRect);
	cube.getGlobalVertex2_z(rect);
	collision=GJK2D(plaRect,rect);

	return collision;
}
void addGroup(const Point3d* const pivSph)
{
	beingAList().push_back(BeingA(pivSph[0],pivSph[1]));
	beingBList().push_back(BeingB(pivSph[2],pivSph[3]));
	beingGList().push_back(BeingG(pivSph[4],pivSph[5]));
	beingRList().push_back(BeingR(pivSph[6],pivSph[7]));
	groupList().push_back(Group(&beingRList().back(),&beingGList().back(),&beingBList().back(),&beingAList().back()));
	beingAList().back().setGroupPtr(&groupList().back());
	beingBList().back().setGroupPtr(&groupList().back());
	beingGList().back().setGroupPtr(&groupList().back());
	beingRList().back().setGroupPtr(&groupList().back());
}
void countDown(int time)
{
	int i=time;
	for(;0< i;i--)
	{
		cout<<i<<endl;
		CAP_pause_spin(1000);
	}
	cout<<i<<endl<<endl;
}
void displayScene()
{
	double xyzMM[6];
	xyzMM[0]=-constv().sceneX-constv().sceneOffset;
	xyzMM[1]=-xyzMM[0];
	xyzMM[2]=-constv().sceneY-constv().sceneOffset;
	xyzMM[3]=-xyzMM[2];
	xyzMM[4]=-constv().sceneZ;
	xyzMM[5]=constv().sceneZ;
	drawGround(xyzMM);
	drawWall(xyzMM);
}
void drawCube(int mode,const double* const p)
{
	Point3d p0(p[0],p[1],p[2]),
		p1(-p[0],p[1],p[2]),
		p2(-p[0],-p[1],p[2]),
		p3(p[0],-p[1],p[2]),
		p4(p[0],p[1],-p[2]),
		p5(-p[0],p[1],-p[2]),
		p6(-p[0],-p[1],-p[2]),
		p7(p[0],-p[1],-p[2]);
	
	glBegin(mode);
	vertex3i(p0.get());
	vertex3i(p3.get());
	vertex3i(p7.get());
	vertex3i(p4.get());
	glEnd();
	glBegin(mode);
	vertex3i(p1.get());
	vertex3i(p2.get());
	vertex3i(p6.get());
	vertex3i(p5.get());
	glEnd();
	glBegin(mode);
	vertex3i(p0.get());
	vertex3i(p1.get());
	vertex3i(p5.get());
	vertex3i(p4.get());
	glEnd();
	glBegin(mode);
	vertex3i(p2.get());
	vertex3i(p3.get());
	vertex3i(p7.get());
	vertex3i(p6.get());
	glEnd();
	glBegin(mode);
	vertex3i(p0.get());
	vertex3i(p1.get());
	vertex3i(p2.get());
	vertex3i(p3.get());
	glEnd();
	glBegin(mode);
	vertex3i(p4.get());
	vertex3i(p5.get());
	vertex3i(p6.get());
	vertex3i(p7.get());
	glEnd();
}
void drawGG()
{
	OglPPMatrix oglPPMatrix;
	glTranslatef(canvasWidth/2,canvasHeight/2+128,0);

	double pRect[2]={200,25};
	glDepthMask(GL_FALSE);
	glColor3f(0,0,0);
	drawRectangular(GL_QUADS,pRect);

	glLineWidth(2);
	glTranslatef(-190,-20,0);
	glScalef(0.4,0.4,1);
	glColor3f(1,1,1);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'G');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'O');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'O');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'D');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,' ');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'G');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'A');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'M');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'E');
	glLineWidth(1);
	glDepthMask(GL_TRUE);
}
void drawGround(const double* const xyzMM)
{
	OglPPMatrix oglPPMatrix;
	glTranslatef(0,0,-constv().sceneZ);

	glColor4f(0.15,0.15,0.15,0);
	glBegin(GL_QUADS);
	glVertex3f(xyzMM[1],xyzMM[3],0);
	glVertex3f(xyzMM[0],xyzMM[3],0);
	glVertex3f(xyzMM[0],xyzMM[2],0);
	glVertex3f(xyzMM[1],xyzMM[2],0);
	glEnd();
}
void drawRectangular(int mode,const double* const p)
{
	Point2d p0(p[0],p[1]),
		p1(-p[0],p[1]),
		p2(-p[0],-p[1]),
		p3(p[0],-p[1]);
	glBegin(mode);
	vertex2i(p0.set());
	vertex2i(p1.set());
	vertex2i(p2.set());
	vertex2i(p3.set());
	glEnd();
}
void drawSign()
{
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2i(canvasWidth/2-2,canvasHeight/2+1);
	glVertex2i(canvasWidth/2-16,canvasHeight/2+1);
	glVertex2i(canvasWidth/2+2,canvasHeight/2);
	glVertex2i(canvasWidth/2+16,canvasHeight/2);
	glVertex2i(canvasWidth/2,canvasHeight/2-2);
	glVertex2i(canvasWidth/2,canvasHeight/2-16);
	glVertex2i(canvasWidth/2+1,canvasHeight/2+2);
	glVertex2i(canvasWidth/2+1,canvasHeight/2+16);
	glColor3f(1,1,1);
	glVertex2i(canvasWidth/2-2,canvasHeight/2);
	glVertex2i(canvasWidth/2-16,canvasHeight/2);
	glVertex2i(canvasWidth/2+2,canvasHeight/2+1);
	glVertex2i(canvasWidth/2+16,canvasHeight/2+1);
	glVertex2i(canvasWidth/2+1,canvasHeight/2-2);
	glVertex2i(canvasWidth/2+1,canvasHeight/2-16);
	glVertex2i(canvasWidth/2,canvasHeight/2+2);
	glVertex2i(canvasWidth/2,canvasHeight/2+16);
	glEnd();
}
void drawWall(const double* const xyzMM)
{
	glColor4f(0.5,0.5,0.5,0);
	glBegin(GL_QUADS);
	glVertex3f(xyzMM[0],xyzMM[3],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[2],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[2],xyzMM[4]);
	glVertex3f(xyzMM[0],xyzMM[3],xyzMM[4]);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(xyzMM[1],xyzMM[3],xyzMM[5]);
	glVertex3f(xyzMM[1],xyzMM[2],xyzMM[5]);
	glVertex3f(xyzMM[1],xyzMM[2],xyzMM[4]);
	glVertex3f(xyzMM[1],xyzMM[3],xyzMM[4]);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(xyzMM[1],xyzMM[2],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[2],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[2],xyzMM[4]);
	glVertex3f(xyzMM[1],xyzMM[2],xyzMM[4]);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(xyzMM[1],xyzMM[3],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[3],xyzMM[5]);
	glVertex3f(xyzMM[0],xyzMM[3],xyzMM[4]);
	glVertex3f(xyzMM[1],xyzMM[3],xyzMM[4]);
	glEnd();
}
void drawWin()
{
	OglPPMatrix oglPPMatrix;
	glTranslatef(canvasWidth/2,canvasHeight/2+128,0);

	double pRect[2]={160,25};
	glDepthMask(GL_FALSE);
	glColor3f(0,0,0);
	drawRectangular(GL_QUADS,pRect);

	glLineWidth(2);
	glTranslatef(-150,-20,0);
	glScalef(0.4,0.4,1);
	glColor3f(1,1,1);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'Y');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'O');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'U');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,' ');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'W');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'I');
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'N');
	glLineWidth(1);
	glDepthMask(GL_TRUE);
}
void gameOver(int value)
{
	/**/
	printf("GAME OVER\n");
	if(gameState==2){printf("WIN\n");}
	else if(gameState==3){printf("GOOD GAME\n");}
	printf("Press O quit game\n");
	while(true)
	{
		char press=getch();
		switch(press)
		{
			case 'O':
			case 'o':exit(0);break;
		}
	}
	
}
void gamePause(int value)
{
	gameState=1;
	printf("Game pause\n");
	printf("Press mouse-left shoot\n");
	printf("Press mouse-right laser\n");
	printf("Press WASD move\n");
	printf("Press shift flash\n");
	printf("Press C toggle top-down view\n");
	printf("\n");
	printf("Press G lose 1 HP\n");
	printf("Press H get 1 HP\n");
	printf("Press K add 1 group\n");
	printf("Press M set %d HP\n", INT_MAX);
	printf("Press N toggle automatic\n");
	printf("\n");
	printf("Press O quit game\n");
	printf("Press P pause game\n");
	printf("Press S start game\n");
	printf("\n");
	printf("Please do not move the window \"OGLFPS\"\n");
	printf("Please ensure the middle of window \"OGLFPS\" is not blocked by any other window when start the game\n");
	printf("Anything happens, press Alt+F4 to quit\n");
	printf("Or, press Ctrl+Alt+Del to open task manager\n");
	printf("When count down is 0, game started\n");
	while(true)
	{
		char press=getch();
		switch(press)
		{
			case 'C':
			case 'c':toggleGameMode();break;
			case 'G':
			case 'g':player().setHP(player().getHP()-1);
				printf("Now you have %d HP\n", player().getHP());break;
			case 'H':
			case 'h':player().setHP(player().getHP()+1);
				printf("Now you have %d HP\n", player().getHP());break;
			case 'K':
			case 'k':generateGroup();break;
			case 'L':
			case 'l':print();break;
			case 'M':
			case 'm':player().setHP(INT_MAX);
				printf("Now you have %d HP\n", player().getHP());break;
			case 'N':
			case 'n':player().setAutomatic(!player().getAutomatic());break;
			case 'O':
			case 'o':exit(0);break;
			case 'S':
			case 's':SetCursorPos(constv().sightAnchorX,constv().sightAnchorY);
				countDown(2);
				frameStart=GetTickCount();
				gameState=0;
				return;
				break;
			default:break;
		}
	}
}
void generateGroup()
{
	Point3d pivSph[8];
	pivSph[1][0]=1;
	pivSph[3][0]=1;
	pivSph[5][0]=1;
	pivSph[7][0]=1;

	generatePivSph(pivSph);
	addGroup(pivSph);
}
void generatePivSph(Point3d* const pivSph)
{
	double pivot[2]={0};
	randomSceneCoor2D(pivot);
	pivSph[0].set(pivot[0]+1000,pivot[1]+1000,0);
	pivSph[2].set(pivot[0]-1000,pivot[1]+1000,0);
	pivSph[4].set(pivot[0]-1000,pivot[1]-1000,0);
	pivSph[6].set(pivot[0]+1000,pivot[1]-1000,0);
}
void globalRGB()
{
	glLineWidth(4);

	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3i(1048576,0,0);
	glVertex3i(-1048576,0,0);
	glColor3f(0,1,0);
	glVertex3i(0,1048576,0);
	glVertex3i(0,-1048576,0);
	glColor3f(0,0,1);
	glVertex3i(0,0,1048576);
	glVertex3i(0,0,-1048576);
	glEnd();
	
	Point3d p0(100,100,100),
		p1(0,100,100),
		p2(0,0,100),
		p3(100,0,100),
		p4(100,100,0),
		p5(0,100,0),
		p6(0,0,0),
		p7(100,0,0);
	glColor3f(1,0,0);
	glBegin(GL_QUADS);
	vertex3i(p0.get());
	vertex3i(p3.get());
	vertex3i(p7.get());
	vertex3i(p4.get());
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_QUADS);
	vertex3i(p0.get());
	vertex3i(p1.get());
	vertex3i(p5.get());
	vertex3i(p4.get());
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_QUADS);
	vertex3i(p0.get());
	vertex3i(p1.get());
	vertex3i(p2.get());
	vertex3i(p3.get());
	glEnd();
	
	glLineWidth(1);
}
void isGameOver()
{
	if(beingAList().empty() && beingBList().empty() && beingGList().empty() && beingRList().empty()){goodGame=1;}
	else if(player().getHP()<=0){goodGame=2;}
	else{goodGame=0;}
}
void polarRight(const double* const polar,double* const right)
{
	right[0]=1;
	right[1]=static_cast<int>(polar[1]+270)%PERIGON;
}
void polar_rectangularRight(const double* const polar,double* const right)
{
	double polRt[2];
	polarRight(polar,polRt);
	polar_rectangular(polRt,right);
}
void randomCoor2D(double* const coor2D)
{
	coor2D[0]=rand();
	if(rand()%2){coor2D[0]*=-1;}
	coor2D[1]=rand();
	if(rand()%2){coor2D[1]*=-1;}
}
void randomSceneArea(int* const areaIndex)
{
	static int areaXSize=constv().sceneX/constv().randomMax,
		areaYSize=constv().sceneY/constv().randomMax;
	
	for(int i=0;i< areaXSize;i++)
	{
		if(rand()%2)
		{
			areaIndex[0]=i;
			break;
		}
	}
	for(int i=0;i< areaYSize;i++)
	{
		if(rand()%2)
		{
			areaIndex[1]=i;
			break;
		}
	}
}
void randomSceneCoor2D(double* const coor2D)
{
	int areaIndex[2]={0};

	randomSceneArea(areaIndex);
	if(constv().randomMax< constv().sceneX){coor2D[0]=(rand()+constv().randomMax*areaIndex[0])+constv().orthoX;}
	else{coor2D[0]=rand()%constv().sceneX+constv().orthoX;}
	if(rand()%2){coor2D[0]*=-1;}
	if(constv().randomMax< constv().sceneY){coor2D[1]=(rand()+constv().randomMax*areaIndex[1])+constv().orthoY;}
	else{coor2D[0]=rand()%constv().sceneX+constv().orthoX;}
	if(rand()%2){coor2D[1]*=-1;}
}
void spherical_rectangularRight(const double* const spherical,double* const right)
{
	double sphRt[3];
	sphericalRight(spherical,sphRt);
	spherical_rectangular(sphRt,right);
}
void spherical_rectangularUp(const double* const spherical,double* const up)
{
	double sphUp[3];
	sphericalUp(spherical,sphUp);
	spherical_rectangular(sphUp,up);
}
void sphericalRight(const double* const spherical,double* const right)
{
	right[0]=1;
	right[1]=static_cast<int>(spherical[1]+270)%PERIGON;
	right[2]=90;
}
void sphericalUp(const double* const spherical,double* const up)
{
	up[0]=1;
	up[1]=spherical[1];
	up[2]=static_cast<int>(spherical[2]+270)%PERIGON;
}
void toggleGameMode()
{
	if(gameMode==FPS)
	{
		player().setPivot()[2]=0;
		player().setCamera().setSpherical()[2]=90;
		gameMode=TOP_DOWN_SHOOTER;
	}
	else if(gameMode==TOP_DOWN_SHOOTER)
	{
		SetCursorPos(constv().sightAnchorX,constv().sightAnchorY);
		gameMode=FPS;
	}
}
