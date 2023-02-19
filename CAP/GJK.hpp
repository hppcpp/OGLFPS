/*
#include<vector>

#include"CAP_GLT.hpp"
*/
namespace CAP
{
int GJK2D(const std::vector<Point2d>&,const std::vector<Point2d>&);
void MinkowskiDifference2D_normMin(const std::vector<int>&,const std::vector<int>&,const std::vector<Point2d>&,const std::vector<Point2d>&,Point2d* const);
void MinkowskiDifferenceSet2D(const std::vector<Point2d>&,const std::vector<Point2d>&,std::vector<Point2d>&);
void vertex2Set_D_IMM(const Point2d&,const std::vector<Point2d>&,std::vector<int>&,std::vector<int>&);
void vertex2Set_D_PMM(const Point2d&,const std::vector<Point2d>&,Point2d* const,Point2d* const);

int GJK2D(const std::vector<Point2d>& convexPolygon1,const std::vector<Point2d>& convexPolygon2)
{
	bool notTriVer=true;
	int index=0,
		zone[3]={4,4,4},
		intTemp[2]={0};
	Point2d direction(1,0),
		triangle[3],
		point2Temp[2];
	std::vector<int> plg1IMax,plg1IMin,plg2IMax,plg2IMin;
	
	if(convexPolygon1.size()< 3 || convexPolygon2.size()< 3){return -1;}
	
	plg1IMin.reserve(convexPolygon1.size());
	plg1IMax.reserve(convexPolygon1.size());
	plg2IMin.reserve(convexPolygon2.size());
	plg2IMax.reserve(convexPolygon2.size());
	vertex2Set_D_IMM(direction,convexPolygon1,plg1IMin,plg1IMax);
	vertex2Set_D_IMM(direction,convexPolygon2,plg2IMin,plg2IMax);
	MinkowskiDifference2D_normMin(plg1IMin,plg2IMax,convexPolygon1,convexPolygon2,&triangle[0]);
	MinkowskiDifference2D_normMin(plg1IMax,plg2IMin,convexPolygon1,convexPolygon2,&triangle[1]);
	
	while(true)
	{
		vectorAdd(2,-1.0,triangle[(index+1)%3].get(),triangle[index].get(),point2Temp[0].set());
		vector2Vertical(point2Temp[0].get(),direction.set());
		vectorNormalize(2,direction.set());
		vectorAdd(2,1.0,triangle[index].get(),direction.get(),point2Temp[0].set());
		intTemp[0]=vector2_pSide(triangle[index].get(),triangle[(index+1)%3].get(),triangle[(index+2)%3].get());
		intTemp[1]=vector2_pSide(triangle[index].get(),triangle[(index+1)%3].get(),point2Temp[0].get());
		intTemp[0]=xorSign(intTemp[0],intTemp[1]);
		if(zone[0]<=3 && !intTemp[0]
			|| 3< zone[0] && intTemp[0])
		{vectorC(2,-1.0,direction.set());}
		
		vertex2Set_D_IMM(direction,convexPolygon1,plg1IMin,plg1IMax);
		vertex2Set_D_IMM(direction,convexPolygon2,plg2IMin,plg2IMax);
		intTemp[0]=plg1IMax.size();
		intTemp[1]=plg2IMin.size();
		for(int i=0;i< intTemp[0];i++)
		{
			for(int j=0;j< intTemp[1];j++)
			{
				vectorAdd(2,-1.0,convexPolygon1[plg1IMax[i]].get(),convexPolygon2[plg2IMin[j]].get(),point2Temp[0].set());
				
				notTriVer=true;
				for(int i=0;i< 3;i++)
				{
					if(point2Temp[0]==triangle[i]){notTriVer=false;}
				}
				if(notTriVer)
				{
					triangle[(index+2)%3]=point2Temp[0];
					zone[0]=outTriangle2D(triangle[0].get(),triangle[1].get(),triangle[2].get(),vecZeroPtr<2,double>());
					if(zone[0]==0){return 1;}
					if(zone[0]< 0){return 0;}
					if(zone[0]!=zone[1])
					{
						point2Temp[1]=point2Temp[0];
						zone[2]=zone[0];
					}
				}
			}
		}
		if(zone[1]==zone[2]){return 0;}
		triangle[(index+2)%3]=point2Temp[1];
		zone[0]=zone[2];
		zone[1]=zone[2];
		index=(zone[0]-1)%3;
	}
	
	return INT_MIN;
}
void MinkowskiDifference2D_normMin(const std::vector<int>& plg1I,const std::vector<int>& plg2I,
	const std::vector<Point2d>& polygon1,const std::vector<Point2d>& polygon2,
	Point2d* const result)
{
	double norm=0,normMin=LLONG_MAX;
	int size1=plg1I.size(),size2=plg2I.size();
	Point2d difference;
	
	for(int i=0;i< size1;i++)
	{
		for(int j=0;j< size2;j++)
		{
			vectorAdd(2,-1.0,polygon1[plg1I[i]].get(),polygon2[plg2I[j]].get(),difference.set());
			norm=vectorNorm(2,difference.get());
			if(norm< normMin)
			{
				*result=difference;
				normMin=norm;
			}
		}
	}
}
void MinkowskiDifferenceSet2D(const std::vector<Point2d>& polygon1,const std::vector<Point2d>& polygon2,std::vector<Point2d>& result)
{
	int size1=polygon1.size(),size2=polygon2.size();
	
	result.resize(size1*size2);
	static_cast<std::vector<Point2d> >(result).swap(result);
	for(int i=0;i< size1;i++)
	{
		for(int j=0;j< size2;j++){vectorAdd(2,-1.0,polygon1[i].get(),polygon2[j].get(),result[i*size2+j].set());}
	}
}
//please ensure the direction is normalized
void vertex2Set_D_IMM(const Point2d& direction,const std::vector<Point2d>& polygon,std::vector<int>& plgIMin,std::vector<int>& plgIMax)
{
	int size=polygon.size();
	double product=0,productMax=0,productMin=0;
	
	product=vectorDot(2,polygon[0].get(),direction.get());
	productMin=product;
	plgIMin.resize(0);
	plgIMin.push_back(0);
	productMax=product;
	plgIMax.resize(0);
	plgIMax.push_back(0);
	for(int i=1;i< size;i++)
	{
		product=vectorDot(2,polygon[i].get(),direction.get());
		if(product< productMin)
		{
			plgIMin.resize(0);
			productMin=product;
		}
		else if(productMax< product)
		{
			plgIMax.resize(0);
			productMax=product;
		}
		if(product==productMin){plgIMin.push_back(i);}
		if(product==productMax){plgIMax.push_back(i);}
	}
}
//please ensure the direction is normalized
void vertex2Set_D_PMM(const Point2d& direction,const std::vector<Point2d>& polygon,Point2d* const pMin,Point2d* const pMax)
{
	int iMax=0,iMin=0,size=polygon.size();
	double product=0,productMax=0,productMin=0;
	
	product=vectorDot(2,polygon[0].get(),direction.get());
	productMin=product;
	productMax=product;
	for(int i=1;i< size;i++)
	{
		product=vectorDot(2,polygon[i].get(),direction.get());
		if(product< productMin)
		{
			iMin=i;
			productMin=product;
		}
		else if(productMax< product)
		{
			iMax=i;
			productMax=product;
		}
	}
	*pMin=polygon[iMin];
	*pMax=polygon[iMax];
}

}
