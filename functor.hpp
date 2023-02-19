struct DisplayBeing:public unary_function<const Being&,void>
{
	void operator()(const Being& being)const
	{
		being.display();
		//being.displayDestination();
	}
};
struct DisplayBullet:public unary_function<const Bullet&,void>
{
	void operator()(const Bullet& bullet)const
	{
		bullet.display();
	}
};
struct DisplayRay:public unary_function<const Ray&,void>
{
	void operator()(const Ray& ray)const
	{
		ray.display();
	}
};
struct HitBeing2D:public binary_function<Being&,Ray* const,void>
{
	void operator()(Being& being,Ray* const ray,double* const rayLength,Point3d* const rayDistance,MeshCube** const cubePtr)
	{
		double length,
			localSpherical[3]={0};
		int hit;
		Point3d p0,p1,pBegin,pCube1,pCube2,pEnd;
		pCube1+=being.getPCube();
		pCube2-=being.getPCube();
		localSpherical[1]=being.getSpherical()[1];
		vector3LocalSpherical(localSpherical,being.getPivot().get(),ray->getPivot().get(),pBegin.set());
		vector3LocalSpherical(localSpherical,being.getPivot().get(),ray->getPDistance().get(),pEnd.set());
		hit=Liang_BarskyClipLine3D(pCube1,pCube2,pBegin,pEnd,&p0,&p1);
		if(0< hit)
		{
			vectorAdd(3,-1.0,p0.get(),pBegin.get(),pEnd.set());
			length=vectorNorm(3,pEnd.get());
			if(length< *rayLength)
			{
				vector3GlobalSpherical(localSpherical,being.getPivot().get(),p0.get(),p1.set());
				ray->setPDistance()=p1;
				ray->setHit(true);
				*cubePtr=&being;
			}
		}
	}
};
struct HitBullet2D:public binary_function<Bullet&,Ray* const,void>
{
	void operator()(Bullet& bullet,Ray* const ray,double* const rayLength,Point3d* const rayDistance,MeshCube** const cubePtr)const
	{
		if(bullet.getHP()<=0){return;}
		
		double length,
			localSpherical[3]={0};
		int hit;
		Point3d p0,p1,pBegin,pCube1,pCube2,pEnd;
		pCube1+=bullet.getPCube();
		pCube2-=bullet.getPCube();
		localSpherical[1]=bullet.getSpherical()[1];
		vector3LocalSpherical(localSpherical,bullet.getPivot().get(),ray->getPivot().get(),pBegin.set());
		vector3LocalSpherical(localSpherical,bullet.getPivot().get(),ray->getPDistance().get(),pEnd.set());
		hit=Liang_BarskyClipLine3D(pCube1,pCube2,pBegin,pEnd,&p0,&p1);
		if(0< hit)
		{
			vectorAdd(3,-1.0,p0.get(),pBegin.get(),pEnd.set());
			length=vectorNorm(3,pEnd.get());
			if(length< *rayLength)
			{
				vector3GlobalSpherical(localSpherical,bullet.getPivot().get(),p0.get(),p1.set());
				ray->setPDistance()=p1;
				ray->setHit(true);
				*cubePtr=&bullet;
			}
		}
	}
};
struct UpdateBeing:public unary_function<Being&,void>
{
	void operator()(Being& being)const
	{
		if(being.getHP()<=0){return;}

		being.aim();
		being.shoot();
		being.decideDestination();
		being.move_destination2D();
		being.limitMovement();

		if(0< player().getHP())
		{
			int collision=0;
			collision=collidePlayer_damage(being);
			if(collision)
			{
				player().setHP(player().getHP()-being.getHP());
				being.setHP(0);
				being.corpse();
			}
		}
	}
};
struct UpdateBullet:public unary_function<Bullet&,void>
{
	void operator()(Bullet& bullet)const
	{
		if(bullet.getHP()<=0){return;}

		bullet.move_distance();

		if(0< player().getHP())
		{
			int collision=0;
			collision=collidePlayer_damage(bullet);
			if(collision)
			{
				if(bullet.getColor()==Player::bulletColor && player().getHP()< Player::HPfull){player().setHP(player().getHP()+bullet.getHP());}
				else if(bullet.getColor()==BeingB::bulletColor){player().setHP(player().getHP()-4);}
				else if(bullet.getColor()==BeingG::bulletColor){player().setHP(player().getHP()-2);}
				else{player().setHP(player().getHP()-1);}
				bullet.setHP(0);
				bullet.corpse();
			}
		}
		
		if(bullet.getColor()==BeingG::bulletColor && 0< bullet.getHP())
		{
			/**/
			for_each(beingAList().begin(),beingAList().end(),bind(collideBeing_heal,placeholders::_1,&bullet));
			if(bullet.getHP()<=0){return;}
			for_each(beingBList().begin(),beingBList().end(),bind(collideBeing_heal,placeholders::_1,&bullet));
			if(bullet.getHP()<=0){return;}
			for_each(beingGList().begin(),beingGList().end(),bind(collideBeing_heal,placeholders::_1,&bullet));
			if(bullet.getHP()<=0){return;}
			for_each(beingRList().begin(),beingRList().end(),bind(collideBeing_heal,placeholders::_1,&bullet));
			if(bullet.getHP()<=0){return;}

		}
	}
};

struct UpdateRay:public unary_function<Ray&,void>
{
	void operator()(Ray& ray)const{}
};
