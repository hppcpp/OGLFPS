void beingInitialization()
{
	Point3d pivSph[8];
	pivSph[1][0]=1;
	pivSph[3][0]=1;
	pivSph[5][0]=1;
	pivSph[7][0]=1;
	/*
	pivSph[0].set(1000,1000,0);
	pivSph[2].set(-1000,1000,0);
	pivSph[4].set(-1000,-1000,0);
	pivSph[6].set(1000,-1000,0);
	addGroup(pivSph);
	*/
	/**/
	int grouAmou=1;
	printf("Challenge group amount: ");
	scanf("%d", &grouAmou);
	printf("\n");
	if(grouAmou< 1){grouAmou=1;}
	for(int i=0;i< grouAmou;i++)
	{
		generatePivSph(pivSph);
		addGroup(pivSph);
	}
	Player::HPfull=grouAmou*Player::gHPfull;
	
}
void earlyUpdate()
{
	keyboard();
	mouse();
	if(gameMode==FPS){player().turn();}
	else if(gameMode==TOP_DOWN_SHOOTER){player().turn_topDown();}
}
void glewInitialization(){}
void glutInitialization()
{
	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glViewport(0,0,canvasWidth,canvasHeight);
	player().look();
	
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}
void initialization()
{
	srand(time(NULL));
	constv();
	
	deltaTime=constv().frameTime;
	sensitivity=0.5;
	canvasHeight=768;
	canvasWidth=1024;
	gameState=1;
	goodGame=0;
	frameIdle=0;
	frameOver=0;
	frameStart=0;
	gameMode=FPS;
	
	beingInitialization();
	player();
	
	windowRatio=canvasWidth/canvasHeight;
	windowPX=(constv().screenWidth-canvasWidth)/2;
	windowPY=(constv().screenHeight-canvasHeight)/2;
	
	//CONSOLE_CURSOR_INFO cursorInfo={1,0};
	//SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);
}
void keyboard()
{
	if(gameMode==FPS)
	{
		/**/
		if(GetAsyncKeyState(VK_LSHIFT)&0x8000 && player().flashReady())
		{
			if(GetAsyncKeyState('A')&0x8000){player().flashLeft2D();}
			if(GetAsyncKeyState('D')&0x8000){player().flashRight2D();}
			if(GetAsyncKeyState('S')&0x8000){player().flashBack2D();}
			if(GetAsyncKeyState('W')&0x8000){player().flashFront2D();}
		}
		else
		{
			if(GetAsyncKeyState('A')&0x8000){player().runLeft2D();}
			if(GetAsyncKeyState('D')&0x8000){player().runRight2D();}
			if(GetAsyncKeyState('S')&0x8000){player().runBack2D();}
			if(GetAsyncKeyState('W')&0x8000){player().runFront2D();}
		}

		/*
		if(GetAsyncKeyState(VK_LSHIFT)&0x8000)
		{
			if(GetAsyncKeyState('A')&0x8000){player().runLeft();}
			if(GetAsyncKeyState('D')&0x8000){player().runRight();}
			if(GetAsyncKeyState('S')&0x8000){player().runBack();}
			if(GetAsyncKeyState('W')&0x8000){player().runFront();}
			if(GetAsyncKeyState('Q')&0x8000){player().runDown();}
			if(GetAsyncKeyState('E')&0x8000){player().runUp();}
		}
		else
		{
			if(GetAsyncKeyState('A')&0x8000){player().walkLeft();}
			if(GetAsyncKeyState('D')&0x8000){player().walkRight();}
			if(GetAsyncKeyState('S')&0x8000){player().walkBack();}
			if(GetAsyncKeyState('W')&0x8000){player().walkFront();}
			if(GetAsyncKeyState('Q')&0x8000){player().walkDown();}
			if(GetAsyncKeyState('E')&0x8000){player().walkUp();}
		}
		*/
	}
	else if(gameMode==TOP_DOWN_SHOOTER)
	{
		/**/
		if(GetAsyncKeyState(VK_LSHIFT)&0x8000 && player().flashReady())
		{
			if(GetAsyncKeyState('A')&0x8000){player().flashA();}
			if(GetAsyncKeyState('D')&0x8000){player().flashD();}
			if(GetAsyncKeyState('S')&0x8000){player().flashS();}
			if(GetAsyncKeyState('W')&0x8000){player().flashW();}
		}
		else
		{
			if(GetAsyncKeyState('A')&0x8000){player().runA();}
			if(GetAsyncKeyState('D')&0x8000){player().runD();}
			if(GetAsyncKeyState('S')&0x8000){player().runS();}
			if(GetAsyncKeyState('W')&0x8000){player().runW();}
		}

		/*
		if(GetAsyncKeyState(VK_LSHIFT)&0x8000)
		{
			if(GetAsyncKeyState('A')&0x8000){player().runA();}
			if(GetAsyncKeyState('D')&0x8000){player().runD();}
			if(GetAsyncKeyState('S')&0x8000){player().runS();}
			if(GetAsyncKeyState('W')&0x8000){player().runW();}
		}
		else
		{
			if(GetAsyncKeyState('A')&0x8000){player().walkA();}
			if(GetAsyncKeyState('D')&0x8000){player().walkD();}
			if(GetAsyncKeyState('S')&0x8000){player().walkS();}
			if(GetAsyncKeyState('W')&0x8000){player().walkW();}
		}
		*/
	}
	player().limitMovement();
}
void lateUpdate()
{
	if(gameMode==FPS){player().look();}
	else if(gameMode==TOP_DOWN_SHOOTER){player().look_topDown();}
}
void mouse()
{
	if(GetAsyncKeyState(VK_LBUTTON)&0x8000)
	{
		if(!player().getTrigger() || player().getAutomatic()){player().shoot();}
	}
	else{player().setTrigger(false);}
	if(GetAsyncKeyState(VK_RBUTTON)&0x8000)
	{
		if(!player().getLaser() && player().laserReady())
		{
			player().setLaser(true);
			player().setLaserSW(GetTickCount());
		}
		if(player().getLaser()){player().shoot();}
	}
	if(player().getLaser() && player().laserOver())
	{
		player().setLaser(false);
		player().setLaserSW(GetTickCount());
	}
}
void update()
{
	if(Player::bulletCD< GetTickCount()-player().getBulletSW())
	{
		Player::generateBlackBullet();
		player().setBulletSW(GetTickCount());
	}
	/*
	beingList().remove_if(beingIsOver);
	*/
	beingAList().remove_if(beingIsOver);
	beingBList().remove_if(beingIsOver);
	beingGList().remove_if(beingIsOver);
	beingRList().remove_if(beingIsOver);
	bulletList().remove_if(bulletIsOver);
	groupList().remove_if(groupIsOver);
	rayList().remove_if(rayIsOver);

	/*
	for_each(beingList().begin(),beingList().end(),UpdateBeing());
	*/
	for_each(beingAList().begin(),beingAList().end(),UpdateBeing());
	for_each(beingBList().begin(),beingBList().end(),UpdateBeing());
	for_each(beingGList().begin(),beingGList().end(),UpdateBeing());
	for_each(beingRList().begin(),beingRList().end(),UpdateBeing());
	for_each(bulletList().begin(),bulletList().end(),UpdateBullet());
	//for_each(rayList().begin(),rayList().end(),UpdateRay());
}
