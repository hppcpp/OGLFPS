//PivotSphere
PivotSphere::PivotSphere(const Point3d& piv,const Point3d& sph):
	pivot(piv),
	spherical(sph)
{}



//Camera
Camera::Camera(const Point3d& piv,const Point3d& sph):
	PivotSphere(piv,sph)
{}

void Camera::look()const
{
	double rectangular[3],up[3];
	spherical_rectangular(spherical.get(),rectangular);
	spherical_rectangularUp(spherical.get(),up);
	glLoadIdentity();
	glFrustum(-10,10,-10,10,10,1048576);
	gluLookAt(pivot[0],pivot[1],pivot[2],
		pivot[0]+rectangular[0],pivot[1]+rectangular[1],pivot[2]+rectangular[2],
		up[0],up[1],up[2]);
}
void Camera::look_topDown(const PivotSphere& tripod)const
{
	glLoadIdentity();
	glOrtho(tripod.getPivot()[0]-constv().orthoX,tripod.getPivot()[0]+constv().orthoX,
		tripod.getPivot()[1]-constv().orthoY,tripod.getPivot()[1]+constv().orthoY,
		-16384,16384);
}
void Camera::look_tripod(const PivotSphere& tripod)const
{
	double direction[3],rectangular[3],up[3];
	direction[0]=1;
	direction[1]=tripod.getSpherical()[1];
	direction[2]=spherical[2];
	spherical_rectangular(direction,rectangular);
	spherical_rectangularUp(direction,up);
	glLoadIdentity();
	glFrustum(-10,10,-10,10,10,1048576);
	gluLookAt(tripod.getPivot()[0],tripod.getPivot()[1],tripod.getPivot()[2]+pivot[2],
		tripod.getPivot()[0]+rectangular[0],tripod.getPivot()[1]+rectangular[1],tripod.getPivot()[2]+pivot[2]+rectangular[2],
		up[0],up[1],up[2]);
}
void Camera::rotate()
{
	POINT pMouse;
	GetCursorPos(&pMouse);
	SetCursorPos(constv().sightAnchorX,constv().sightAnchorY);
	
	double phi=spherical[2]+(pMouse.y-constv().sightAnchorY)*sensitivity,
		theta=spherical[1]+(constv().sightAnchorX-pMouse.x)*sensitivity;
	theta=static_cast<int>(theta+PERIGON)%PERIGON;
	if(phi< 0){phi=0;}
	else if(SA< phi){phi=SA;}
	spherical.set(1,theta,phi);
}
void Camera::rotate_topDown(Point3d& tripodSpherical)
{
	POINT pMouse;
	GetCursorPos(&pMouse);
	
	double polar[2],rectangular[2];
	rectangular[0]=pMouse.x-constv().screenWidthHalf;
	rectangular[1]=constv().screenHeightHalf-pMouse.y;
	rectangular_polar(rectangular,polar);
	tripodSpherical.setTheta(polar[1]);
}
void Camera::rotate_tripod(Point3d& tripodSpherical)
{
	POINT pMouse;
	GetCursorPos(&pMouse);
	SetCursorPos(constv().sightAnchorX,constv().sightAnchorY);
	
	//double phi=spherical[2]+(pMouse.y-constv().sightAnchorY)*sensitivity;
	double theta=tripodSpherical[1]+(constv().sightAnchorX-pMouse.x)*sensitivity;
	theta=static_cast<int>(theta+PERIGON)%PERIGON;
	/*
	if(phi< 0){phi=0;}
	else if(SA< phi){phi=SA;}
	spherical.setPhi(phi);
	*/
	tripodSpherical.setTheta(theta);
}

//MeshCube
MeshCube::MeshCube(const Point3d& piv,const Point3d& sph,const Point3d& pc,const RGBAd& c):
	PivotSphere(piv,sph),
	pCube(pc),
	color(c)
{}

void MeshCube::displayCube_lucency()const
{
	OglPPMatrix oglPPMatrix;

	glTranslatef(pivot[0],pivot[1],pivot[2]);
	glRotatef(spherical[2],0,1,0);
	glRotatef(spherical[1],0,0,1);
	color4f(color.get());
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	drawCube(GL_QUADS,pCube.get());
	glDisable(GL_BLEND);
}
void MeshCube::displayCube_lucency_z(double HPf)const
{
	OglPPMatrix oglPPMatrix;
	
	double HPc=getHP();
	RGBAd lucency=color;
	lucency[3]=1.0-HPc/HPf;
	glTranslatef(pivot[0],pivot[1],pivot[2]);
	glRotatef(spherical[1],0,0,1);
	color4f(lucency.get());
	if(0< HPc)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
		drawCube(GL_QUADS,pCube.get());
		glDisable(GL_BLEND);
	}
	else{drawCube(GL_LINE_LOOP,pCube.get());}
}
void MeshCube::displayCube_z(bool solid=false)const
{
	OglPPMatrix oglPPMatrix;
	
	glTranslatef(pivot[0],pivot[1],pivot[2]);
	glRotatef(spherical[1],0,0,1);
	color4f(color.get());
	if(solid){drawCube(GL_QUADS,pCube.get());}
	else{drawCube(GL_LINE_LOOP,pCube.get());}
}
void MeshCube::getGlobalVertex2_z(vector<Point2d>& vecPt)const
{
	double matrix[4],point[2]={0},pointArr[8]={pCube[0],pCube[1],-pCube[0],pCube[1],-pCube[0],-pCube[1],pCube[0],-pCube[1]};
	vector2RotateMatrix2(spherical[1],matrix);
	vecPt.resize(4);
	for(int i=0;i< 4;i++)
	{
		matrixMultiply(2,2,1,matrix,&pointArr[i*2],point);
		vecPt[i].set(point);
		vectorAddAssign(2,1.0,vecPt[i].set(),pivot.get());
	}
}
void MeshCube::getLocalVertex2(vector<Point2d>& vecPt)const
{
	double pointArr[8]={pCube[0],pCube[1],-pCube[0],pCube[1],-pCube[0],-pCube[1],pCube[0],-pCube[1]};
	vecPt.resize(4);
	for(int i=0;i< 4;i++){vecPt[i].set(&pointArr[i*2]);}
}
void MeshCube::limitMovement()
{
	if(pivot.getx()< -constv().sceneX){pivot.setx(-constv().sceneX);}
	else if(constv().sceneX< pivot.getx()){pivot.setx(constv().sceneX);}
	if(pivot.gety()< -constv().sceneY){pivot.sety(-constv().sceneY);}
	else if(constv().sceneY< pivot.gety()){pivot.sety(constv().sceneY);}
}

//Ray
Ray::Ray(const Point3d& piv,const Point3d& sph):
	PivotSphere(piv,sph),
	hit(false),
	start(GetTickCount()),
	color(1,1,1,0.25)
{
	double rectangular[3];
	spherical_rectangular(spherical.get(),rectangular);
	vectorAdd(3,Ray::length,pivot.get(),rectangular,pDistance.set());
}

inline bool Ray::isOver()const{return rayIsOver(*this);}
void Ray::display()const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	color4f(color.get());
	glBegin(GL_LINES);
	vertex3i(pivot.get());
	vertex3i(pDistance.get());
	glEnd();
	if(hit)
	{
		glPointSize(4);
		glBegin(GL_POINTS);
		vertex3i(pDistance.get());
		glEnd();
		glPointSize(1);
	}
	glDisable(GL_BLEND);
}



//Being
Being::Being(const Point3d& piv,const Point3d& sph,const Point3d& pc,const RGBAd& c,double sp,int hp):
	MeshCube(piv,sph,pc,c),
	//state(st),
	speed(sp),
	HP(hp),
	stopwatch(GetTickCount()),
	groupPtr(0)
{
	spherical[2]=90;
	destination=pivot;
}

int Being::RGBA1()const
{
	int rgba=-1;
	for(int i=0;i< 4;i++)
	{
		if(color[i]==1)
		{
			rgba=i;
			break;
		}
	}
	return rgba;
}
void Being::aim(){aimPlayer2D();}
void Being::aimPlayer2D()
{
	double polar[2];
	Point3d p=player().getPivot();
	p-=pivot;
	rectangular_polar(p.get(),polar);
	spherical[1]=polar[1];
}
inline void Being::corpse(){if(HP<=0){stopwatch=GetTickCount();}}
void Being::decideDestination(){decideDestination_keepDistance(Being::socialDistance);}
void Being::decideDestination_keepDistance(double distance)
{
	double norm;
	Point3d orientation=pivot;
	orientation-=player().getPivot();
	norm=vectorNorm(2,orientation.get());
	orientation*=distance/norm;
	orientation+=player().getPivot();
	setDestination()=orientation;
}
void Being::display()const{displayCube_lucency_z(Being::HPfull);}
void Being::displayDestination()const
{
	color4f(color.get());
	glBegin(GL_LINES);
	vertex3i(player().getPivot().get());
	vertex3i(destination.get());
	glEnd();
	glPointSize(4);
	glBegin(GL_POINTS);
	vertex3i(destination.get());
	glEnd();
	glPointSize(1);
}
void Being::move_destination2D()
{
	if(pivot==destination){return;}
	
	double norm=0;
	Point3d orientation=destination;
	orientation-=pivot;
	norm=vectorNorm(3,orientation.get());
	if(norm<=speed*deltaTime){pivot=destination;}
	else{vectorAddAssign(2,speed*deltaTime/norm,pivot.set(),orientation.get());}
}
void Being::shoot(const Point3d& bCube,const RGBAd& c,double sp,int hp,DWORD spe, bool muzzle=true)const
{
	double length=deltaTime*sp/2,
		polar[2];
	Point3d muzzlePivot=pivot,pc=bCube;
	if(muzzle){muzzlePivot[2]+=constv().muzzleOffset;}
	if(pc[0]< length){pc[0]=length;}
	polar_rectangular(spherical.get(),polar);
	vectorAddAssign(2,-length,muzzlePivot.set(),polar);
	bulletList().push_back(Bullet(muzzlePivot,spherical,pc,c,sp,hp,spe));
}
inline void Being::subtractHP(int subtrahend){HP-=subtrahend;}

//Bullet
Bullet::Bullet(const Point3d& piv,const Point3d& sph,const Point3d& pc,const RGBAd& c,double sp,int hp, DWORD spe=Bullet::existenceSpell):
	MeshCube(piv,sph,pc,c),
	//state(st),
	speed(sp),
	HP(hp),
	spell(spe),
	stopwatch(GetTickCount())
{}

inline void Bullet::corpse(){if(HP<=0){stopwatch=GetTickCount()-spell+Ray::spell;}}
inline void Bullet::display()const
{
	if(color==Player::bulletColor){displayCube_lucency_z(Player::gHPfull);}
	else if(color==BeingB::bulletColor){displayCube_lucency_z(Bullet::cHPfull);}
	else if(color==BeingR::bulletColor){displayCube_lucency_z(Bullet::pHPfull);}
	else if(color==BeingG::bulletColor){displayCube_lucency_z(Bullet::yHPfull);}
	else if(color==BeingA::bulletColor){displayCube_lucency_z(Bullet::gHPfull);}
}
void Bullet::move_distance()
{
	double orientation[3];
	spherical_rectangular(spherical.get(),orientation);
	vectorAddAssign(3,speed*deltaTime,pivot.set(),orientation);
}
inline void Bullet::subtractHP(int subtrahend){HP-=subtrahend;}

//Player
Player::Player(const Point3d& piv,const Point3d& sph,const Point3d& pc,const RGBAd& c,double fs,double rs,double ws,int hp,const Camera& cam):
	MeshCube(piv,sph,pc,c),
	automatic(false),
	laser(false),
	trigger(false),
	//state(st),
	flashSpeed(fs),
	runSpeed(rs),
	walkSpeed(ws),
	HP(hp),
	camera(cam),
	flashSW(GetTickCount()-Player::flashCD),
	laserSW(GetTickCount()-Player::laserCD),
	bulletSW(GetTickCount()-Player::bulletSpell)
{}

void Player::generateBlackBullet()
{
	Point3d piv;

	randomSceneCoor2D(piv.set());
	bulletList().push_back(Bullet(piv,constv().origin,Player::bulletCube,Player::bulletColor,0,Player::gHPfull,Player::bulletSpell));
}

bool Player::flashReady()const
{
	if(Player::flashCD<=GetTickCount()-flashSW){return true;}
	return false;
}
bool Player::laserOver()const
{
	if(Player::laserSpell<=GetTickCount()-laserSW){return true;}
	return false;
}
bool Player::laserReady()const
{
	if(Player::laserCD<=GetTickCount()-laserSW){return true;}
	return false;
}
double Player::getHPbar()const{return 1.0-HP/static_cast<double>(Player::HPfull);}
inline void Player::display()const{displayCube_lucency_z(Player::HPfull);}
void Player::displayFlashBar()const
{
	OglPPMatrix oglPPMatrix;
	glTranslatef(100,50,0);

	double bar=GetTickCount()-flashSW;
	if(bar< Player::flashCD/2){bar=1;}
	else if(Player::flashCD<=bar){bar=0;}
	else{bar=0.5;}

	double pRect[2]={25,25};
	RGBAd lucency(1,1,1,bar);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	color4f(lucency.get());
	drawRectangular(GL_QUADS,pRect);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glLineWidth(2);
	glTranslatef(-20,-20,0);
	glScalef(0.4,0.4,1);
	glColor3f(0,0,0);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'F');
	glLineWidth(1);
}
void Player::displayHPBar()const
{
	OglPPMatrix oglPPMatrix;
	glTranslatef(50,50,0);

	double pRect[2]={25,25};
	RGBAd lucency(1,1,1,getHPbar());
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
	color4f(lucency.get());
	drawRectangular(GL_QUADS,pRect);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glLineWidth(2);
	glTranslatef(-20,-20,0);
	glScalef(0.4,0.4,1);
	glColor3f(0,0,0);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'H');
	glLineWidth(1);
}
void Player::displayLaserBar()const
{
	
	OglPPMatrix oglPPMatrix;
	glTranslatef(150,50,0);

	if(!laser)
	{
		double bar=GetTickCount()-laserSW;
		if(bar< Player::laserCD/2){bar=1;}
		else if(Player::laserCD<=bar){bar=0;}
		else{bar=0.5;}

		double pRect[2]={25,25};
		RGBAd lucency(1,1,1,bar);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
		color4f(lucency.get());
		drawRectangular(GL_QUADS,pRect);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}

	glLineWidth(2);
	glTranslatef(-20,-20,0);
	glScalef(0.4,0.4,1);
	glColor3f(0,0,0);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'L');
	glLineWidth(1);
}
inline void Player::flashBack2D()
{
	moveBack2D(flashSpeed);
	flashSW=GetTickCount();
}
inline void Player::flashFront2D()
{
	moveFront2D(flashSpeed);
	flashSW=GetTickCount();
}
inline void Player::flashLeft2D()
{
	moveLeft2D(flashSpeed);
	flashSW=GetTickCount();
}
inline void Player::flashRight2D()
{
	moveRight2D(flashSpeed);
	flashSW=GetTickCount();
}
void Player::getDirection(double* const direction)const
{
	direction[0]=1;
	direction[1]=spherical[1];
	direction[2]=camera.getSpherical()[2];
}
inline void Player::look()const{camera.look_tripod(*this);}
inline void Player::look_topDown()const{camera.look_topDown(*this);}
void Player::moveBack(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangular(direction,orientation);
	vectorAddAssign(3,-speed*deltaTime,pivot.set(),orientation);
}
void Player::moveBack2D(double speed)
{
	double orientation[2];
	polar_rectangular(spherical.get(),orientation);
	vectorAddAssign(2,-speed*deltaTime,pivot.set(),orientation);
}
void Player::moveDown(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangularUp(direction,orientation);
	vectorAddAssign(3,-speed*deltaTime,pivot.set(),orientation);
}
void Player::moveFront(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangular(direction,orientation);
	vectorAddAssign(3,speed*deltaTime,pivot.set(),orientation);
}
void Player::moveFront2D(double speed)
{
	double orientation[2];
	polar_rectangular(spherical.get(),orientation);
	vectorAddAssign(2,speed*deltaTime,pivot.set(),orientation);
}
void Player::moveLeft(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangularRight(direction,orientation);
	vectorAddAssign(3,-speed*deltaTime,pivot.set(),orientation);
}
void Player::moveLeft2D(double speed)
{
	double orientation[2];
	polar_rectangularRight(spherical.get(),orientation);
	vectorAddAssign(2,-speed*deltaTime,pivot.set(),orientation);
}
void Player::moveRight(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangularRight(direction,orientation);
	vectorAddAssign(3,speed*deltaTime,pivot.set(),orientation);
}
void Player::moveRight2D(double speed)
{
	double orientation[2];
	polar_rectangularRight(spherical.get(),orientation);
	vectorAddAssign(2,speed*deltaTime,pivot.set(),orientation);
}
void Player::moveUp(double speed)
{
	double direction[3],orientation[3];
	getDirection(direction);
	spherical_rectangularUp(direction,orientation);
	vectorAddAssign(3,speed*deltaTime,pivot.set(),orientation);
}
inline void Player::runBack(){moveBack(runSpeed);}
inline void Player::runBack2D(){moveBack2D(runSpeed);}
inline void Player::runDown(){moveDown(runSpeed);}
inline void Player::runFront(){moveFront(runSpeed);}
inline void Player::runFront2D(){moveFront2D(runSpeed);}
inline void Player::runLeft(){moveLeft(runSpeed);}
inline void Player::runLeft2D(){moveLeft2D(runSpeed);}
inline void Player::runRight(){moveRight(runSpeed);}
inline void Player::runRight2D(){moveRight2D(runSpeed);}
inline void Player::runUp(){moveUp(runSpeed);}
void Player::shoot()
{
	trigger=true;
	
	double rayLength=Ray::length;
	Point3d rayPivot=pivot,raySpherical,
		rayDistance;
	MeshCube *cubePtr=0;
	Being *beingPtr=0;
	Bullet *bulletPtr=0;
	rayPivot[2]+=constv().muzzleOffset;
	getDirection(raySpherical.set());
	rayList().push_back(Ray(rayPivot,raySpherical));
	/*
	if(gameMode==FPS)
	{
		camera.setSpherical()[2]-=constv().recoil;
		spherical[1]-=rand()%constv().recoil;
	}
	for_each(beingList().begin(),beingList().end(),bind(HitBeing(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	*/
	for_each(beingAList().begin(),beingAList().end(),bind(HitBeing2D(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	for_each(beingBList().begin(),beingBList().end(),bind(HitBeing2D(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	for_each(beingGList().begin(),beingGList().end(),bind(HitBeing2D(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	for_each(beingRList().begin(),beingRList().end(),bind(HitBeing2D(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	for_each(bulletList().begin(),bulletList().end(),bind(HitBullet2D(),placeholders::_1,&rayList().back(),&rayLength,&rayDistance,&cubePtr));
	if(cubePtr!=0 && 0< cubePtr->getHP())
	{
		beingPtr=dynamic_cast<Being*>(cubePtr);
		if(beingPtr)
		{
			if(0< beingPtr->getHP()){beingPtr->subtractHP(1);}
			beingPtr->corpse();
		}
		bulletPtr=dynamic_cast<Bullet*>(cubePtr);
		if(bulletPtr)
		{
			bulletPtr->subtractHP(1);
			bulletPtr->corpse();
		}
	}
}
inline void Player::turn(){camera.rotate_tripod(spherical);}
inline void Player::turn_topDown(){camera.rotate_topDown(spherical);}
inline void Player::walkBack(){moveBack(walkSpeed);}
inline void Player::walkBack2D(){moveBack2D(walkSpeed);}
inline void Player::walkDown(){moveDown(walkSpeed);}
inline void Player::walkFront(){moveFront(walkSpeed);}
inline void Player::walkFront2D(){moveFront2D(walkSpeed);}
inline void Player::walkLeft(){moveLeft(walkSpeed);}
inline void Player::walkLeft2D(){moveLeft2D(walkSpeed);}
inline void Player::walkRight(){moveRight(walkSpeed);}
inline void Player::walkRight2D(){moveRight2D(walkSpeed);}
inline void Player::walkUp(){moveUp(walkSpeed);}

inline void Player::flashA()
{
	pivot[0]-=flashSpeed*deltaTime;
	flashSW=GetTickCount();
}
inline void Player::flashD()
{
	pivot[0]+=flashSpeed*deltaTime;
	flashSW=GetTickCount();
}
inline void Player::flashS()
{
	pivot[1]-=flashSpeed*deltaTime;
	flashSW=GetTickCount();
}
inline void Player::flashW()
{
	pivot[1]+=flashSpeed*deltaTime;
	flashSW=GetTickCount();
}
inline void Player::runA(){pivot[0]-=runSpeed*deltaTime;}
inline void Player::runD(){pivot[0]+=runSpeed*deltaTime;}
inline void Player::runS(){pivot[1]-=runSpeed*deltaTime;}
inline void Player::runW(){pivot[1]+=runSpeed*deltaTime;}
inline void Player::walkA(){pivot[0]-=walkSpeed*deltaTime;}
inline void Player::walkD(){pivot[0]+=walkSpeed*deltaTime;}
inline void Player::walkS(){pivot[1]-=walkSpeed*deltaTime;}
inline void Player::walkW(){pivot[1]+=walkSpeed*deltaTime;}



//BeingA
BeingA::BeingA(const Point3d& piv,const Point3d& sph):
	Being(piv,sph,Point3d(Being::PCX,Being::PCY,constv().sceneZ),RGBAd(0.25,0.25,0.25,1),10,BeingA::HPfull)
{}

void BeingA::decideDestination()
{
	double norm;
	Point3d orientation,vertical;
	if(getGroupPtr()->beingPtr[2] && 0< getGroupPtr()->beingPtr[2]->getHP()){orientation=getGroupPtr()->beingPtr[2]->getPivot();}
	else if(getGroupPtr()->beingPtr[0] && 0< getGroupPtr()->beingPtr[0]->getHP()){orientation=getGroupPtr()->beingPtr[0]->getPivot();}
	else
	{
		decideDestination_keepDistance(BeingA::keepDistance);
		return;
	}
	orientation-=player().getPivot();
	vector2Vertical(orientation.get(),vertical.set());
	norm=vectorNorm(2,vertical.get());
	vertical*=BeingA::keepDistance/norm;
	vertical+=player().getPivot();
	setDestination()=vertical;
}
void BeingA::display()const{displayCube_lucency_z(BeingA::HPfull);}
void BeingA::shoot()
{
	if(GetTickCount()-getStopwatch()< BeingA::shootSpell){return;}
	
	Being::shoot(BeingA::bulletCube,BeingA::bulletColor,50,Bullet::gHPfull,BeingA::bulletSpell);
	setStopwatch(GetTickCount());
}

//BeingB
BeingB::BeingB(const Point3d& piv,const Point3d& sph):
	Being(piv,sph,Point3d(Being::PCX,Being::PCY,constv().sceneZ),RGBAd(0,0,1,0),7.5,BeingB::HPfull)
{}

void BeingB::decideDestination()
{
	double norm;
	Point3d orientation=pivot;
	orientation-=player().getPivot();
	norm=vectorNorm(2,orientation.get());
	orientation*=BeingB::keepDistance/norm;
	orientation+=player().getPivot();
	setDestination()=orientation;
}
void BeingB::display()const{displayCube_lucency_z(BeingB::HPfull);}
void BeingB::shoot()
{
	if(GetTickCount()-getStopwatch()< BeingB::shootSpell){return;}
	
	Being::shoot(BeingB::bulletCube,BeingB::bulletColor,10,Bullet::cHPfull,BeingB::bulletSpell, false);
	setStopwatch(GetTickCount());
}

//BeingG
BeingG::BeingG(const Point3d& piv,const Point3d& sph):
	Being(piv,sph,Point3d(Being::PCX,Being::PCY,constv().sceneZ),RGBAd(0,1,0,0),10,BeingG::HPfull)
{}

void BeingG::aim()
{
	double polar[2];
	Point3d orientation;
	if(getGroupPtr()->beingPtr[2] && getGroupPtr()->beingPtr[2]->getHP()< Being::HPfull){orientation=getGroupPtr()->beingPtr[2]->getPivot();}
	else if(getGroupPtr()->beingPtr[3] && getGroupPtr()->beingPtr[3]->getHP()< Being::HPfull){orientation=getGroupPtr()->beingPtr[3]->getPivot();}
	else if(getGroupPtr()->beingPtr[0] && getGroupPtr()->beingPtr[0]->getHP()< Being::HPfull){orientation=getGroupPtr()->beingPtr[0]->getPivot();}
	else
	{
		aimPlayer2D();
		return;
	}
	orientation-=pivot;
	rectangular_polar(orientation.get(),polar);
	spherical[1]=polar[1];
}
void BeingG::decideDestination()
{
	double norm;
	Point3d orientation;
	if(getGroupPtr()->beingPtr[2] && 0< getGroupPtr()->beingPtr[2]->getHP()){orientation=getGroupPtr()->beingPtr[2]->getPivot();}
	else if(getGroupPtr()->beingPtr[3] && 0< getGroupPtr()->beingPtr[3]->getHP()){orientation=getGroupPtr()->beingPtr[3]->getPivot();}
	else if(getGroupPtr()->beingPtr[0] && 0< getGroupPtr()->beingPtr[0]->getHP()){orientation=getGroupPtr()->beingPtr[0]->getPivot();}
	else
	{
		decideDestination_keepDistance(BeingG::keepDistance);
		return;
	}
	orientation-=player().getPivot();
	norm=vectorNorm(2,orientation.get());
	orientation*=(BeingG::keepDistance+norm)/norm;
	orientation+=player().getPivot();
	setDestination()=orientation;
}
void BeingG::display()const{displayCube_lucency_z(BeingG::HPfull);}
void BeingG::shoot()
{
	if(GetTickCount()-getStopwatch()< BeingG::shootSpell){return;}

	Being::shoot(BeingG::bulletCube,BeingG::bulletColor,15,Bullet::yHPfull,BeingG::bulletSpell);
	setStopwatch(GetTickCount());
}

//BeingR
BeingR::BeingR(const Point3d& piv,const Point3d& sph):
	Being(piv,sph,Point3d(Being::PCX,Being::PCY,constv().sceneZ),RGBAd(1,0,0,0),15,BeingR::HPfull),
	lastDestination(GetTickCount()-BeingR::lastDestinationSpell)
{}

void BeingR::decideDestination()
{
	if(GetTickCount()-lastDestination<=BeingR::lastDestinationSpell){return;}
	lastDestination=GetTickCount();

	double norm;
	Point3d orientation;
	randomCoor2D(orientation.set());
	orientation-=player().getPivot();
	norm=vectorNorm(2,orientation.set());
	orientation*=BeingR::keepDistance/norm;
	orientation+=player().getPivot();
	setDestination()=orientation;
}
void BeingR::display()const{displayCube_lucency_z(BeingR::HPfull);}
void BeingR::shoot()
{
	if(GetTickCount()-getStopwatch()< BeingR::shootSpell){return;}
	
	Being::shoot(BeingR::bulletCube,BeingR::bulletColor,25,Bullet::pHPfull,BeingR::bulletSpell);
	setStopwatch(GetTickCount());
}
