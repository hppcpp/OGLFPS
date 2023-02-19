void constvPrint()
{
	printf("const\n");
	printf("frameTime==%d\n", constv().frameTime);
	printf("screenHeight==%d\n", constv().screenHeight);
	printf("screenHeightHalf==%d\n", constv().screenHeightHalf);
	printf("screenWidth==%d\n", constv().screenWidth);
	printf("screenWidthHalf==%d\n", constv().screenWidthHalf);
}
void frameTimePrint()
{
	printf("deltaTime==%lu\n", deltaTime);
	printf("frameOver-frameStart==%lu\n", frameOver-frameStart);
}
template<typename TN>
void matrixPrint(int M,int N,const TN* const mat)
{
	for(int i=0;i< M;i++){vectorPrint(N,&mat[i*N]);}
	cout<<endl;
}
void print()
{
	/*
	printf("position==");
	vectorPrint(3,player().getPivot().get());
	printf("spherical==");
	vectorPrint(3,player().getSpherical().get());
	printf("groupSize  %d\n", groupList().size());
	*/
	printf("playerHP  %d\n", player().getHP());
}
void staticvPrint()
{
	printf("static\n");
	printf("sensitivity==%lf\n", sensitivity);
	printf("windowRatio==%lf\n", windowRatio);
	printf("canvasHeight==%d\n", canvasHeight);
	printf("canvasWidth==%d\n", canvasWidth);
	printf("deltaTime==%lu\n", deltaTime);
	printf("frameStart==%lu\n", frameStart);
	printf("frameOver==%lu\n", frameOver);
}
template<typename TN>
void vectorPrint(int dimension,const TN* const vec)
{
	for(int i=0;i< dimension;i++){cout<<vec[i]<<"  ";}
	cout<<endl;
}
template<typename TN>
void vectorPrint_index(const vector<int>& vecI,const vector<TN>& vec)
{
	int size=vecI.size();
	for(int i=0;i< size;i++){vectorPrint(2,vec[vecI[i]].get());}
}
