enum GAME_MODE{FPS=1,TOP_DOWN_SHOOTER};

double sensitivity,windowRatio;
int canvasHeight,canvasWidth,gameState,goodGame,windowPX,windowPY;
DWORD deltaTime,frameIdle,frameOver,frameStart;
GAME_MODE gameMode;

inline const double* vec3dZero(){return vecZeroPtr<3,double>();}
inline Constv& constv()
{
	static Constv value;
	return value;
}
inline Player& player()
{
	static Player value(Point3d(0,0,0),Point3d(1,0,0),
		Point3d(Being::PCX,Being::PCY,constv().sceneZ),RGBAd(1,1,1,0),
		200,10,5,
		Player::HPfull,
		Camera(Point3d(0,0,constv().muzzleOffset+125),Point3d(1,0,90)));
	return value;
}

/*
inline list<Being>& beingList()
{
	static list<Being> value;
	return value;
}
*/
inline list<BeingA>& beingAList()
{
	static list<BeingA> value;
	return value;
}
inline list<BeingB>& beingBList()
{
	static list<BeingB> value;
	return value;
}
inline list<BeingG>& beingGList()
{
	static list<BeingG> value;
	return value;
}
inline list<BeingR>& beingRList()
{
	static list<BeingR> value;
	return value;
}
inline list<Bullet>& bulletList()
{
	static list<Bullet> value;
	return value;
}
inline list<Group>& groupList()
{
	static list<Group> value;
	return value;
}
inline list<Ray>& rayList()
{
	static list<Ray> value;
	return value;
}



const double Being::PCX=100;
const double Being::PCY=250;
const double Being::socialDistance=2000;
const int Being::HPfull=4;
const DWORD Being::corpseSpell=10000;

const double BeingA::keepDistance=8000;
const int BeingA::HPfull=4;
const DWORD BeingA::bulletSpell=4000;
const DWORD BeingA::shootSpell=4000;
const Point3d BeingA::bulletCube(100,10,10);
const RGBAd BeingA::bulletColor(0.8,0.8,0.8,0);

const double BeingB::keepDistance=5000;
const int BeingB::HPfull=8;
const DWORD BeingB::bulletSpell=30000;
const DWORD BeingB::shootSpell=6000;
const Point3d BeingB::bulletCube(5,Being::PCY*3,constv().sceneZ);
const RGBAd BeingB::bulletColor(0,1,1,1);

const double BeingG::keepDistance=6000;
const int BeingG::HPfull=4;
const DWORD BeingG::bulletSpell=16000;
const DWORD BeingG::shootSpell=4000;
const Point3d BeingG::bulletCube(100,100,100);
const RGBAd BeingG::bulletColor(1,1,0,1);

const double BeingR::keepDistance=3000;
const int BeingR::HPfull=6;
const DWORD BeingR::bulletSpell=8000;
const DWORD BeingR::lastDestinationSpell=1000;
const DWORD BeingR::shootSpell=2000;
const Point3d BeingR::bulletCube(20,20,20);
const RGBAd BeingR::bulletColor(1,0,1,1);

const int Bullet::cHPfull=15;
const int Bullet::gHPfull=1;
const int Bullet::pHPfull=2;
const int Bullet::yHPfull=5;
const DWORD Bullet::existenceSpell=15000;

int Player::HPfull=8;
const int Player::gHPfull=8;
const DWORD Player::bulletCD=5000;
const DWORD Player::bulletSpell=30000;
const DWORD Player::flashCD=2000;
const DWORD Player::laserCD=2000;
const DWORD Player::laserSpell=1000;
const Point3d Player::bulletCube(100,100,100);
const RGBAd Player::bulletColor(0,0,0,0);

const double Ray::length=65536;
const DWORD Ray::spell=1000;
