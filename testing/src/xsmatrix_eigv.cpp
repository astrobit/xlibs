#include<cstdio>
#include<cstdlib>
#include <xlinalg.h>


int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	xsquare_matrix_long mA(3);

	mA.Set(0,0,1);
	mA.Set(0,1,-3);
	mA.Set(0,2,3);

	mA.Set(1,0,3);
	mA.Set(1,1,-5);
	mA.Set(1,2,3);

	mA.Set(2,0,6);
	mA.Set(2,1,-6);
	mA.Set(2,2,4);

	xvector_long vlEig = mA.Get_Eigenvector(4.0);
	vlEig.Print();

	mA.Set(0,0,0);
	mA.Set(0,1,1);
	mA.Set(0,2,1);

	mA.Set(1,0,1);
	mA.Set(1,1,0);
	mA.Set(1,2,1);

	mA.Set(2,0,1);
	mA.Set(2,1,1);
	mA.Set(2,2,0);

	vlEig = mA.Get_Eigenvector(2.0);
	vlEig.Print();
	return 0;
}
