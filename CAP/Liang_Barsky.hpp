/*
#include<algorithm>
#include<map>
#include<vector>

#include"CAP_GLT.hpp"
*/
namespace CAP
{
int Liang_BarskyClipLine3D(const CAP::Point3d&,const CAP::Point3d&,const CAP::Point3d&,const CAP::Point3d&,CAP::Point3d* const,CAP::Point3d* const);

int Liang_BarskyClipLine3D(const CAP::Point3d& pCube1,const CAP::Point3d& pCube2,
	const CAP::Point3d& pBegin,const CAP::Point3d& pEnd,
	CAP::Point3d* const p0,CAP::Point3d* const p1)
{
	int amnt=0,i=0,size=0;
	double dx=0,dy=0,dz=0,
		uTemp=0;
	
	if(pCube1==pCube2 || pBegin==pEnd || p0==0 || p1==0){return -1;}
	
	dx=pEnd.getx()-pBegin.getx();
	dy=pEnd.gety()-pBegin.gety();
	dz=pEnd.getz()-pBegin.getz();
	
	CAP::Point3d pMin,pMax;
	CAP::vectorMM(3,pCube1.get(),pCube2.get(),pMin.set(),pMax.set());
	
	std::vector<double> u;
	std::map<double,CAP::Point3d> up;
	u.reserve(8);
	u.push_back(0);
	up.insert(std::make_pair(0,pBegin));
	u.push_back(1);
	up.insert(std::make_pair(1,pEnd));
	if(dx!=0)
	{
		uTemp=(pMin.getx()-pBegin.getx())/dx;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pMin.getx(),pBegin.gety()+uTemp*dy,pBegin.getz()+uTemp*dz)));
		}
		uTemp=(pMax.getx()-pBegin.getx())/dx;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pMax.getx(),pBegin.gety()+uTemp*dy,pBegin.getz()+uTemp*dz)));
		}
	}
	if(dy!=0)
	{
		uTemp=(pMin.gety()-pBegin.gety())/dy;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pBegin.getx()+uTemp*dx,pMin.gety(),pBegin.getz()+uTemp*dz)));
		}
		uTemp=(pMax.gety()-pBegin.gety())/dy;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pBegin.getx()+uTemp*dx,pMax.gety(),pBegin.getz()+uTemp*dz)));
		}
	}
	if(dz!=0)
	{
		uTemp=(pMin.getz()-pBegin.getz())/dz;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pBegin.getx()+uTemp*dx,pBegin.gety()+uTemp*dy,pMin.getz())));
		}
		uTemp=(pMax.getz()-pBegin.getz())/dz;
		if(0<=uTemp && uTemp<=1)
		{
			u.push_back(uTemp);
			up.insert(std::make_pair(uTemp,CAP::Point3d(pBegin.getx()+uTemp*dx,pBegin.gety()+uTemp*dy,pMax.getz())));
		}
	}
	sort(u.begin(),u.end());
	u.erase(unique(u.begin(),u.end()),u.end());
	amnt=0;
	size=u.size();
	for(i=0;i< size && amnt< 2;i++)
	{
		if(CAP::vectorBetweenEqual(3,pMin.get(),pMax.get(),up[u[i]].get()))
		{
			if(amnt==0){*p0=up[u[i]];}
			if(amnt==1){*p1=up[u[i]];}
			amnt++;
		}
	}

	return amnt;
}

}
