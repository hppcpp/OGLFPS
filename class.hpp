class Constsv;
class PivotSphere;

class Camera;
class Constv;
class MeshCube;
class Ray;

class Being;
class Bullet;
class Player;

class BeingB;
class BeingG;
class BeingR;

struct Group;



class Constsv
{
	public:
		const int screenHeight,screenWidth,randomMax;
		
		Constsv():
			screenHeight(GetSystemMetrics(SM_CYSCREEN)),
			screenWidth(GetSystemMetrics(SM_CXSCREEN)),
			randomMax(pow(2,15))
		{}
};
class PivotSphere
{
	protected:
		Point3d pivot,spherical;
	
	public:
		PivotSphere(){}
		explicit PivotSphere(const Point3d&,const Point3d&);
		virtual ~PivotSphere(){}
		
		const Point3d& getPivot()const{return pivot;}
		const Point3d& getSpherical()const{return spherical;}
		Point3d& setPivot(){return pivot;}
		Point3d& setSpherical(){return spherical;}
};

class Camera:public PivotSphere
{
	public:
		Camera(){}
		explicit Camera(const Point3d&,const Point3d&);
		virtual ~Camera(){}
		
		void look()const;
		void look_topDown(const PivotSphere&)const;
		void look_tripod(const PivotSphere&)const;
		void rotate();
		void rotate_topDown(Point3d&);
		void rotate_tripod(Point3d&);
};
class Constv:public Constsv
{
	public:
		const int frameTime,
			muzzleOffset,
			orthoX,orthoY,
			recoil,recoilX,recoilY,
			sceneOffset,sceneX,sceneY,sceneZ,
			screenHeightHalf,screenWidthHalf,
			sightAnchorX,sightAnchorY;
		const Point3d origin;
		
		Constv():
			frameTime(100/6),
			muzzleOffset(500),
			orthoX(6144),
			orthoY(4096),
			recoil(2),
			recoilX(2),
			recoilY(3),
			sceneOffset(128),
			sceneX(65536),
			sceneY(65536),
			sceneZ(875),
			screenHeightHalf(screenHeight/2),
			screenWidthHalf(screenWidth/2),
			sightAnchorX(screenWidth/2),
			sightAnchorY(screenHeight/2+64),
			origin(vecZeroPtr<3,double>())
		{}
};
class MeshCube:public PivotSphere
{
	protected:
		Point3d pCube;
		RGBAd color;
	public:
		explicit MeshCube(const Point3d&,const Point3d&,const Point3d&,const RGBAd&);
		virtual ~MeshCube(){}
		
		virtual void corpse(){}
		void displayCube_lucency()const;
		void displayCube_lucency_z(double)const;
		void displayCube_z(bool)const;
		void getGlobalVertex2_z(vector<Point2d>&)const;
		void getLocalVertex2(vector<Point2d>&)const;
		void limitMovement();
		
		virtual int getHP()const{return INT_MIN;}
		virtual void setHP(int value){}
		const Point3d& getPCube()const{return pCube;}
		Point3d& setPCube(){return pCube;}
		const RGBAd& getColor()const{return color;}
		RGBAd& setColor(){return color;}
};
class Ray:public PivotSphere
{
	protected:
		bool hit;
		const DWORD start;
		Point3d pDistance;
		RGBAd color;
	public:
		static const double length;
		static const DWORD spell;
		
		explicit Ray(const Point3d&,const Point3d&);
		
		inline bool isOver()const;
		void display()const;
		
		bool getHit()const{return hit;}
		void setHit(bool value){hit=value;}
		DWORD getStart()const{return start;}
		const Point3d& getPDistance()const{return pDistance;}
		Point3d& setPDistance(){return pDistance;}
		const RGBAd& getColor()const{return color;}
		RGBAd& setColor(){return color;}
};

class Being:public MeshCube
{
	private:
		//char state;
		double speed;
		int HP;
		DWORD stopwatch;
		Point3d destination;
		Group* groupPtr;
	public:
		static const int HPfull;
		static const double PCX,PCY,socialDistance;
		static const DWORD corpseSpell;
		
		explicit Being(const Point3d&,const Point3d&,const Point3d&,const RGBAd&,double,int);
		
		int RGBA1()const;
		virtual void aim();
		void aimPlayer2D();
		inline void corpse();
		virtual void decideDestination();
		void decideDestination_keepDistance(double);
		virtual void display()const;
		void displayDestination()const;
		void move_destination2D();
		virtual void shoot(){}
		void shoot(const Point3d&,const RGBAd&,double,int,DWORD, bool)const;
		inline void subtractHP(int);
		
		//char getState()const{return state;}
		int getHP()const{return HP;}
		void setGroupPtr(Group* const value){groupPtr=value;}
		void setHP(int value){HP=value;}
		//void setState(char value){state=value;}
		void setStopwatch(DWORD value){stopwatch=value;}
		DWORD getStopwatch()const{return stopwatch;}
		const Group* getGroupPtr(){return groupPtr;}
		Group* setGroup(){return groupPtr;}
		const Point3d& getDestination()const{return destination;}
		Point3d& setDestination(){return destination;}
};
class Bullet:public MeshCube
{
	private:
		//char state;
		double speed;
		int HP;
		DWORD spell,stopwatch;
	public:
		static const int cHPfull,gHPfull,pHPfull,yHPfull;
		static const DWORD existenceSpell;
		
		explicit Bullet(const Point3d&,const Point3d&,const Point3d&,const RGBAd&,double,int, DWORD);
		
		inline void corpse();
		inline void display()const;
		void move_distance();
		inline void subtractHP(int);
		
		//char getState()const{return state;}
		int getHP()const{return HP;}
		void setHP(int value){HP=value;}
		void setSpell(DWORD value){spell=value;}
		//void setState(char value){state=value;}
		void setStopwatch(DWORD value){stopwatch=value;}
		DWORD getSpell()const{return spell;}
		DWORD getStopwatch()const{return stopwatch;}
};
class Player:public MeshCube
{
	protected:
		bool automatic,laser,trigger;
		//char state;
		double flashSpeed,runSpeed,walkSpeed;
		int HP;
		Camera camera;
		DWORD flashSW,laserSW,bulletSW;
	public:
		static int HPfull;
		static const int gHPfull;
		static const DWORD bulletCD,bulletSpell,flashCD,laserCD,laserSpell;
		static const Point3d bulletCube;
		static const RGBAd bulletColor;

		static void generateBlackBullet();

		explicit Player(const Point3d&,const Point3d&,const Point3d&,const RGBAd&,double,double,double,int,const Camera&);
		
		bool flashReady()const;
		bool laserOver()const;
		bool laserReady()const;
		double getHPbar()const;
		inline void display()const;
		void displayFlashBar()const;
		void displayLaserBar()const;
		void displayHPBar()const;
		inline void flashBack2D();
		inline void flashFront2D();
		inline void flashLeft2D();
		inline void flashRight2D();
		void getDirection(double* const)const;
		inline void look()const;
		inline void look_topDown()const;
		void moveBack(double);
		void moveBack2D(double);
		void moveDown(double);
		void moveFront(double);
		void moveFront2D(double);
		void moveLeft(double);
		void moveLeft2D(double);
		void moveRight(double);
		void moveRight2D(double);
		void moveUp(double);
		inline void runBack();
		inline void runBack2D();
		inline void runDown();
		inline void runFront();
		inline void runFront2D();
		inline void runLeft();
		inline void runLeft2D();
		inline void runRight();
		inline void runRight2D();
		inline void runUp();
		void shoot();
		inline void turn();
		inline void turn_topDown();
		inline void walkBack();
		inline void walkBack2D();
		inline void walkDown();
		inline void walkFront();
		inline void walkFront2D();
		inline void walkLeft();
		inline void walkLeft2D();
		inline void walkRight();
		inline void walkRight2D();
		inline void walkUp();
		
		inline void flashA();
		inline void flashD();
		inline void flashS();
		inline void flashW();
		inline void runA();
		inline void runD();
		inline void runS();
		inline void runW();
		inline void walkA();
		inline void walkD();
		inline void walkS();
		inline void walkW();
		
		bool getAutomatic()const{return automatic;}
		bool getLaser()const{return laser;}
		bool getTrigger()const{return trigger;}
		//char getState()const{return state;}
		double getFlashSpeed()const{return flashSpeed;}
		double getRunSpeed()const{return runSpeed;}
		double getWalkSpeed()const{return walkSpeed;}
		int getHP()const{return HP;}
		void setAutomatic(bool value){automatic=value;}
		void setBulletSW(DWORD value){bulletSW=value;}
		void setFlashSpeed(double value){flashSpeed=value;}
		void setHP(int value){HP=value;}
		void setLaser(bool value){laser=value;}
		void setLaserSW(DWORD value){laserSW=value;}
		void setRunSpeed(double value){walkSpeed=value;}
		//void setState(char value){state=value;}
		void setTrigger(bool value){trigger=value;}
		void setWalkSpeed(double value){walkSpeed=value;}
		const Camera& getCamera()const{return camera;}
		Camera& setCamera(){return camera;}
		DWORD getBulletSW()const{return bulletSW;}
		DWORD getFlashSW()const{return flashSW;}
		DWORD getLaserSW()const{return laserSW;}
};

class BeingA:public Being
{
	public:
		static const double keepDistance;
		static const int HPfull;
		static const DWORD bulletSpell;
		static const DWORD shootSpell;
		static const Point3d bulletCube;
		static const RGBAd bulletColor;

		explicit BeingA(const Point3d&,const Point3d&);

		void decideDestination();
		void display()const;
		void shoot();
};
class BeingB:public Being
{
	public:
		static const double keepDistance;
		static const int HPfull;
		static const DWORD bulletSpell,shootSpell;
		static const Point3d bulletCube;
		static const RGBAd bulletColor;

		explicit BeingB(const Point3d&,const Point3d&);

		void decideDestination();
		void display()const;
		void shoot();
};
class BeingG:public Being
{
	public:
		static const double keepDistance;
		static const int HPfull;
		static const DWORD bulletSpell,shootSpell;
		static const Point3d bulletCube;
		static const RGBAd bulletColor;

		explicit BeingG(const Point3d&,const Point3d&);

		void aim();
		void decideDestination();
		void display()const;
		void shoot();
};
class BeingR:public Being
{
	private:
		DWORD lastDestination;
	public:
		static const double keepDistance;
		static const int HPfull;
		static const DWORD bulletSpell,lastDestinationSpell,shootSpell;
		static const Point3d bulletCube;
		static const RGBAd bulletColor;

		explicit BeingR(const Point3d&,const Point3d&);

		void decideDestination();
		void display()const;
		void shoot();
};

struct Group
{
	Being* beingPtr[4];

	Group(BeingR* const rPtr,
		BeingG* const gPtr,
		BeingB* const bPtr,
		BeingA* const aPtr):
		beingPtr{rPtr,gPtr,bPtr,aPtr}
	{}
};
