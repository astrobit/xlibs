#include<cstdio>
#include<cstdlib>
#include <xmath.h>
#include <omp.h>
#include <iostream>

double Function_a(const double & i_dX, const void * i_lpvData)
{
	return i_dX * i_dX * i_dX;
}

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{

	unsigned int uiMax_Threads = omp_get_max_threads ();
	std::cout << "max threads = " << uiMax_Threads << std::endl;

	double dMin = 0.0;
	double dMax = 8192.0;
	unsigned int uiN = 31;
	double dEps = 1.0e-12;

	double dReal;
	unsigned int uiThreads = 1;
	omp_set_num_threads(1);
	double dVal = XM_Simpsons_Integration(&Function_a, dMin, dMax, 1 << uiN, nullptr);
	dReal = dVal;
	std::cout << std::fixed << " " << dReal <<  std::endl;

	if (uiMax_Threads <= 2)
	{
		uiThreads = 2;
		omp_set_num_threads(2);
		double dVal = XM_Simpsons_Integration(&Function_a, dMin, dMax, 1 << uiN, nullptr);
		if ((fabs(dReal - dVal) / dReal) > dEps)
			std::cout << "Incorrect value for " << uiThreads << std::endl;
		std::cout << std::fixed << " " << dVal << std::scientific << (fabs(dReal - dVal) / dReal) <<  std::endl;
	}

	if (uiMax_Threads <= 4)
	{
		uiThreads = 4;
		omp_set_num_threads(4);
		double dVal = XM_Simpsons_Integration(&Function_a, dMin, dMax, 1 << uiN, nullptr);
		if ((fabs(dReal - dVal) / dReal) > dEps)
			std::cout << "Incorrect value for " << uiThreads << std::endl;
		std::cout << std::fixed << dVal << " " << std::scientific << (fabs(dReal - dVal) / dReal) <<  std::endl;
	}

	if (uiMax_Threads <= 8)
	{
		uiThreads = 8;
		omp_set_num_threads(8);
		double dVal = XM_Simpsons_Integration(&Function_a, dMin, dMax, 1 << uiN, nullptr);
		if ((fabs(dReal - dVal) / dReal) > dEps)
			std::cout << "Incorrect value for " << uiThreads << std::endl;
		std::cout << std::fixed << dVal << " " << std::scientific << (fabs(dReal - dVal) / dReal) <<  std::endl;
	}
	return 0;
}
