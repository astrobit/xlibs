#include "xmath.h"
#include "xcomplex.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cmath>

xmath_constants	g_cConstants;

xmath_constants::xmath_constants(void)
{
	dPi = acos(-1.0);
	dE = exp(1.0);
	dLn2 = log(2.0);
}

double	g_dConvergence_Threhold = 0.001;
void XMATH_Set_Convergence_Threshold(const double &dValue)
{
	if (dValue > 0.0 && dValue < 100.0)
		g_dConvergence_Threhold = dValue;
}
double XMATH_Get_Convergence_Threshold(void)
{
	return g_dConvergence_Threhold;
}

//XMATH_Delta_Threshold_Test
// Function to test if a delta is below a user specified threshold 
inline bool XMATH_Delta_Threshold_Test(const double &i_dX, const double  &i_dDelta, unsigned int uiMaximum_Log_Diff)
{
	int iXP;
	int iDeltaP;

	frexp(i_dX,&iXP);
	frexp(i_dDelta,&iDeltaP);

	return (i_dDelta == 0.0 || (i_dX != 0.0 && (unsigned int)abs(iXP - iDeltaP) >= uiMaximum_Log_Diff));
}

inline bool XMATH_Delta_Threshold_Test(const xcomplex &cX, const xcomplex &cDelta)
{
	return (XMATH_Delta_Threshold_Test(cX.dRe,cDelta.dRe) && XMATH_Delta_Threshold_Test(cX.dIm,cDelta.dIm));
}
inline bool XMATH_Threshold_Test(const xcomplex &xcX1, const xcomplex &xcX2, const double &dThreshold_Pct)
{
	double	dMod_X1 = xcX1.modulus();
	double	dDelta_Mod = (xcX2 - xcX1).modulus();
	double	dErr = dDelta_Mod / dMod_X1 * 100.0;
	return (dErr < dThreshold_Pct);
}

xcomplex XM_Simpsons_Integration(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, unsigned int uiNumSteps, const void * i_lpvData)
{
	xcomplex	xcStep = (xcEnd - xcStart) / uiNumSteps;
	xcomplex	xcIntegral(0.0,0.0);

	
	xcIntegral = lpfFn(xcStart,i_lpvData) + lpfFn(xcEnd,i_lpvData);
//#pragma omp parallel for reduction(+:xcIntegral)
	for (unsigned int uiIndex = 1; uiIndex < uiNumSteps; uiIndex++)
	{
		xcomplex	xcX = xcStart + xcStep * uiIndex;
		xcIntegral += ((uiIndex & 1) * 2 + 2) * lpfFn(xcX,i_lpvData);
	}
	xcIntegral *= xcStep / 3.0;

	return xcIntegral;
}

double XM_Simpsons_Integration(QFunctionD lpfFn, const double &dStart, const double &dEnd, unsigned int uiNumSteps, const void * i_lpvData)
{
	double	dStep = (dEnd - dStart) / uiNumSteps;
	double		dIntegral(0.0);

	
	dIntegral = lpfFn(dStart,i_lpvData) + lpfFn(dEnd,i_lpvData);
#pragma omp parallel for reduction(+:dIntegral)
	for (unsigned int uiIndex = 1; uiIndex < uiNumSteps; uiIndex++)
	{
		double	dX = dStart + dStep * uiIndex;
		dIntegral += ((uiIndex & 1) * 2 + 2) * lpfFn(dX,i_lpvData);
	}
	dIntegral *= dStep / 3.0;

	return dIntegral;
}

long double XM_Simpsons_Integration_LD(QFunctionLD lpfFn, const long double &dStart, const long double &dEnd, unsigned int uiNumSteps, const void * i_lpvData)
{
	long double	dStep = (dEnd - dStart) / uiNumSteps;
	long double		dIntegral(0.0);
	long double dThird((long double)(1.0) / (long double)(3.0));

	
	dIntegral = lpfFn(dStart,i_lpvData) + lpfFn(dEnd,i_lpvData);
#pragma omp parallel for reduction(+:dIntegral)
	for (unsigned int uiIndex = 1; uiIndex < uiNumSteps; uiIndex++)
	{
		long double	dX = dStart + dStep * uiIndex;
		dIntegral += ((uiIndex & 1) * 2 + 2) * lpfFn(dX,i_lpvData);
	}
	dIntegral *= dStep * dThird;

	return dIntegral;
}

double	g_dXMNR_Trapezoid_Res(0.0);
double XMNR_Trapezoid(QFunctionD lpfFn, const double &i_dStart, const double &i_dEnd, unsigned int uiN, const void * i_lpvData)
{
	double	dDelta = (i_dEnd - i_dStart);
	if (uiN == 1)
		g_dXMNR_Trapezoid_Res = 0.5 * dDelta * (lpfFn(i_dStart,i_lpvData) + lpfFn(i_dEnd,i_lpvData));
	else
	{
		unsigned int uiIT = 1, uiJ;
		for (uiJ = 1; uiJ < uiN - 1; uiJ++) uiIT <<= 1;
		double dDel = dDelta / uiIT;
		double dX0 = i_dStart + 0.5 * dDel;
		double dSum = 0.0;
		for (uiJ = 0; uiJ < uiIT; uiJ++) dSum += lpfFn(dDel * uiJ + dX0,i_lpvData);
		g_dXMNR_Trapezoid_Res = 0.5 *(g_dXMNR_Trapezoid_Res + dDelta * dSum / uiIT);
	}
	return g_dXMNR_Trapezoid_Res;
}
double XMNR_Q_Trapezoid(QFunctionD lpfFn, const double &i_dStart, const double &i_dEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	double dS_Last;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ < JMAX && !bQuit; uiJ++)
	{
		double dS = XMNR_Trapezoid(lpfFn,i_dStart,i_dEnd,uiJ,i_lpvData);
		if (uiJ > 5)
		{
			if (XMATH_Delta_Threshold_Test(dS,dS - dS_Last,15)) // check for value within 0.01 %
			{
				bQuit = true;
			}
		}
		dS_Last = dS;
	}
	if (!bQuit)
		dS_Last = 0.0;
	return dS_Last;
}

double XM_Simpsons_Integration_Fast(QFunctionD lpfFn, const double &i_dStart, const double &i_dEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	double	dSt_Last(0.0);
	double	dS_Last(0.0);
	double	dS;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ <= JMAX && !bQuit; uiJ++)
	{
		double	dSt = XMNR_Trapezoid(lpfFn,i_dStart,i_dEnd,uiJ, i_lpvData);
		dS = (4.0 * dSt - dSt_Last) / 3.0;
		if (uiJ > 10)
		{
			if (XMATH_Delta_Threshold_Test(dS,dS - dS_Last,15)) // check for value within 0.01 %
			{
				bQuit = true;
			}
		}
		dSt_Last = dSt;
		dS_Last = dS;
	}
	if (!bQuit)
		dS_Last = 0.0;
	return dS_Last;
}

xcomplex	g_XMNR_Trapezoid_Res(0.0,0.0);
xcomplex XMNR_Trapezoid(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, unsigned int uiN, const void * i_lpvData)
{
	xcomplex	xcDelta = (xcEnd - xcStart);
	if (uiN == 1)
		g_XMNR_Trapezoid_Res = 0.5 * xcDelta * (lpfFn(xcStart,i_lpvData) + lpfFn(xcEnd,i_lpvData));
	else
	{
		unsigned int uiIT = 1, uiJ;
		for (uiJ = 1; uiJ < uiN - 1; uiJ++) uiIT <<= 1;
		xcomplex xcDel = xcDelta / uiIT;
		xcomplex xcX0 = xcStart + 0.5 * xcDel;
		xcomplex xcSum = xcomplex(0.0,0.0);
		for (uiJ = 0; uiJ < uiIT; uiJ++) xcSum += lpfFn(xcDel * uiJ + xcX0,i_lpvData);
		g_XMNR_Trapezoid_Res = 0.5 *(g_XMNR_Trapezoid_Res + xcDelta * xcSum / uiIT);
	}
	return g_XMNR_Trapezoid_Res;
}
xcomplex XMNR_Q_Trapezoid(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	xcomplex xcS_Last;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ < JMAX && !bQuit; uiJ++)
	{
		xcomplex xcS = XMNR_Trapezoid(lpfFn,xcStart,xcEnd,uiJ,i_lpvData);
		if (uiJ > 5)
		{
			if (XMATH_Threshold_Test(xcS,xcS_Last,g_dConvergence_Threhold)) // check for value within 0.01 %
			{
				bQuit = true;
			}
		}
		xcS_Last = xcS;
	}
	if (!bQuit)
		xcS_Last = xcomplex(0.0,0.0);
	return xcS_Last;
}

xcomplex XM_Simpsons_Integration_Fast(QFunctionC lpfFn, const xcomplex &xcStart, const xcomplex &xcEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	xcomplex	xcSt_Last(0.0,0.0);
	xcomplex	xcS_Last(0.0,0.0);
	xcomplex	xcS;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ <= JMAX && !bQuit; uiJ++)
	{
		xcomplex	xcSt = XMNR_Trapezoid(lpfFn,xcStart,xcEnd,uiJ, i_lpvData);
		xcS = (4.0 * xcSt - xcSt_Last) / 3.0;
		if (uiJ > 10)
		{
			if (XMATH_Threshold_Test(xcS,xcS_Last,g_dConvergence_Threhold)) // check for value within 0.01 %
			{
				bQuit = true;
			}
		}
		xcSt_Last = xcSt;
		xcS_Last = xcS;
	}
	if (!bQuit)
		xcS_Last = xcomplex(0.0,0.0);
	return xcS_Last;
}


double	XM_Runge_Kutte(const xcomplex &xcX, QFunctionC lpfFn, const double &dStep, const void * i_lpvData)
{
	double	dRes;
	xcomplex	cF = lpfFn( xcX,i_lpvData ).sqrt();
	double		dPhi1 = cF.argument();
	xcomplex	xcX2 = xcX + xcomplex(cos(dPhi1),sin(-dPhi1)) * dStep * 0.5;
	xcomplex	cF2 = lpfFn( xcX2,i_lpvData ).sqrt();
	double		dPhi2 = cF2.argument();
	xcomplex	xcX3 = xcX + xcomplex(cos(dPhi2),sin(-dPhi2)) * dStep * 0.5;
	xcomplex	cF3 = lpfFn( xcX3,i_lpvData ).sqrt();
	double		dPhi3 = cF3.argument();
	xcomplex	xcX4 = xcX + xcomplex(cos(dPhi3),sin(-dPhi3)) * dStep;
	xcomplex	cF4 = lpfFn( xcX4,i_lpvData ).sqrt();
	double		dPhi4 = cF4.argument();
	dRes = -(dPhi1 + 2.0 * (dPhi2 + dPhi3) + dPhi4) / 6.0;

	return dRes;
}
double	XM_Runge_Kutte_Init(const xcomplex &xcX, QFunctionC lpfFn, const double &dPhi1, const double &dStep, const void * i_lpvData)
{
	double	dRes;
	xcomplex	xcX2 = xcX + xcomplex(cos(dPhi1),sin(-dPhi1)) * dStep * 0.5;
	xcomplex	cF2 = lpfFn( xcX2,i_lpvData ).sqrt();
	double		dPhi2 = cF2.argument();
	xcomplex	xcX3 = xcX + xcomplex(cos(dPhi2),sin(-dPhi2)) * dStep * 0.5;
	xcomplex	cF3 = lpfFn( xcX3,i_lpvData ).sqrt();
	double		dPhi3 = cF3.argument();
	xcomplex	xcX4 = xcX + xcomplex(cos(dPhi3),sin(-dPhi3)) * dStep;
	xcomplex	cF4 = lpfFn( xcX4,i_lpvData ).sqrt();
	double		dPhi4 = cF4.argument();
	dRes = -(dPhi1 + 2.0 * (dPhi2 + dPhi3) + dPhi4) / 6.0;
	return dRes;
}

unsigned int XFactFast(unsigned int i_uiX)
{
	unsigned int uiRet = 1;
	unsigned int uiI;
	for (uiI = i_uiX; uiI > 1; uiI--)
		uiRet *= uiI;
	return uiRet;
}
void XFactSmall(unsigned int i_uiX, unsigned long long &o_uiResult, unsigned int &o_uiResult_Exponent)
{
	unsigned int uiI;
	unsigned int uiLcl;
	o_uiResult = 1;
	o_uiResult_Exponent = 0;
	if (i_uiX <= 25) // this algorithm works only for x <= 25
	{
		for (uiI = 2; uiI <= i_uiX; uiI++)
		{
			uiLcl = uiI;
			while (!(uiLcl & 0x01))
			{
				o_uiResult_Exponent++;
				uiLcl >>= 1;
			}
			o_uiResult *= uiLcl;
			while (!(o_uiResult & 0x01))
			{
				o_uiResult_Exponent++;
				o_uiResult >>= 1;
			}
		}
	}
}
// XFactLarge: variant on the XFactSmall algorithm to estimate factorials for large X
void XFactLarge(unsigned int i_uiX, unsigned long long &o_uiResult, unsigned int &o_uiResult_Exponent)
{
	unsigned int uiI;
	unsigned int uiLcl;
	unsigned long long uiTemp;
	unsigned long long uiTemp2;
	unsigned long long uiTemp3;
	unsigned long long uiTemp4;
	unsigned long long uiStart;
	unsigned int uiShiftCount = 1;
	o_uiResult = 1;
	o_uiResult_Exponent = 0;
	for (uiI = 2; uiI <= i_uiX; uiI++)
	{
		if (uiI >> (uiShiftCount + 1))
			uiShiftCount++;

		uiLcl = uiI;
		while (!(uiLcl & 0x01))
		{
			o_uiResult_Exponent++;
			uiLcl >>= 1;
		}
		uiTemp2 = 1;
		do
		{
			uiStart = o_uiResult;
			uiTemp = (o_uiResult >> uiShiftCount) * uiLcl;
			o_uiResult *= uiLcl;
			uiTemp3 = o_uiResult >> (64 - uiShiftCount);
			uiTemp4 = uiTemp >> (64 - 2*uiShiftCount);
			uiTemp2 = (uiTemp4 - uiTemp3);
			if (uiTemp2)
			{
				unsigned int uiShift = 0;
				uiTemp3 = uiTemp2 >> uiShiftCount;
				do
				{
					uiShift++;
					uiTemp3 >>= 1;
				}
				while (uiTemp3);
				o_uiResult = uiStart >> uiShift;
				o_uiResult_Exponent+=uiShift;
			}
		}
		while (uiTemp2);

		while (!(o_uiResult & 0x01))
		{
			o_uiResult_Exponent++;
			o_uiResult >>= 1;
		}
	}
}
unsigned long long XFactUI(unsigned int i_uiX)
{
	unsigned long long uiResult;
	unsigned int uiExponent;
	XFactSmall(i_uiX,uiResult, uiExponent);	
	return (uiResult << uiExponent);
}

double	XFactRamunjan(unsigned int i_uiX)
{
	double	dRet = 1.0;
	if (i_uiX > 1)
	{
		// Ramanujan approximation
		double	dLnX = log((double)i_uiX) - 1.0;
		double	dX = i_uiX;
		double	dX1 = ((i_uiX << 3) | 0x04);
		double	dLnB = log(dX * (1 + dX * dX1)) / 6.0;
	//	double	dLnPi_2 = 0.57236494292470008707171367567653;
		dRet = exp(i_uiX * dLnX + dLnB + 0.57236494292470008707171367567653);
	}
	return dRet;
}
double	XFact(unsigned int i_uiX)
{
	double	dRet = 1.0;
	if (i_uiX <= 12)
	{
		dRet = (double)XFactFast(i_uiX);
	}
	else if (i_uiX <= 25)
	{
		unsigned long long uiResult;
		unsigned int uiExponent;
		XFactSmall(i_uiX,uiResult,uiExponent);
		double	dMan = (double)uiResult;
		dRet = ldexp(dMan,(int)uiExponent);
	}
	else
	{
		dRet = XFactRamunjan(i_uiX);
	}
	return dRet;
}

void XBracket(const double &i_dX, const double * i_lpdX, unsigned int i_uiNum_Data_Points, unsigned int &o_uiLower, unsigned int &o_uiUpper)
{
	o_uiLower = 0x0ffffffff;
	o_uiUpper = 0x0ffffffff;
	if (i_uiNum_Data_Points > 2 && i_lpdX)
	{
		bool bLow_To_High = i_lpdX[i_uiNum_Data_Points - 1] > i_lpdX[0];
		if (bLow_To_High)
		{
			if (i_lpdX[0] < i_dX && i_dX < i_lpdX[i_uiNum_Data_Points - 1])
			{
				o_uiLower = 0;
				o_uiUpper = i_uiNum_Data_Points - 1;
				unsigned int uiTest = (o_uiUpper + o_uiLower) / 2;
				while ((o_uiUpper - o_uiLower) > 1)
				{
					if (i_lpdX[uiTest] > i_dX)
					{
						o_uiUpper = uiTest;
					}
					else
					{
						o_uiLower = uiTest;
					}
					uiTest = (o_uiUpper + o_uiLower) / 2;
				}
			}
		}
		else
		{
			if (i_lpdX[0] > i_dX && i_dX > i_lpdX[i_uiNum_Data_Points - 1])
			{
				o_uiLower = 0;
				o_uiUpper = i_uiNum_Data_Points - 1;
				unsigned int uiTest = (o_uiUpper + o_uiLower) / 2;
				while ((o_uiUpper - o_uiLower) > 1)
				{
					if (i_lpdX[uiTest] < i_dX)
					{
						o_uiUpper = uiTest;
					}
					else
					{
						o_uiLower = uiTest;
					}
					uiTest = (o_uiUpper + o_uiLower) / 2;
				}
			}
		}
	}
}

class MININAN
{
public:
	double k_dNAN;

	MININAN(void)
	{
		unsigned int k_luNAN[2];
		k_luNAN[0]=0xffffffff;
		k_luNAN[1]=0x7fffffff;
                void * lpNAN = k_luNAN; // to avoid warnings
        double * lpdTemp = (double *) (lpNAN );
        k_dNAN = lpdTemp[0];
	}
};
MININAN g_kcNAN;

// Linear interpolation
double XInterpolate_Linear(const double &i_dX, const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points)
{
	unsigned int uiUpper;
	unsigned int uiLower;
	double	dY = g_kcNAN.k_dNAN;
	XBracket(i_dX,i_lpdX,i_uiNum_Data_Points,uiLower,uiUpper);
	if (uiLower != 0x0ffffffff && uiUpper != 0x0ffffffff)
	{
		dY = (i_lpdY[uiUpper] - i_lpdY[uiLower]) / (i_lpdX[uiUpper] - i_lpdX[uiLower]) * (i_dX - i_lpdX[uiLower]);
	}
	return dY;
}
double XInterpolate_CSpline(const double & i_dX, const double i_lpdX[3], const double i_lpdY[3])
{
	double	dY = g_kcNAN.k_dNAN;
	if ((i_dX > i_lpdX[0] && i_dX < i_lpdX[2] && i_lpdX[1] > i_lpdX[0] && i_lpdX[1] < i_lpdX[2]) || 
		(i_dX < i_lpdX[0] && i_dX > i_lpdX[2] && i_lpdX[1] < i_lpdX[0] && i_lpdX[1] > i_lpdX[2]))
	{
		double dSecond_Derivative = 2.0 * ((i_lpdY[0] - i_lpdY[1]) / (i_lpdX[0] - i_lpdX[1]) - (i_lpdY[2] - i_lpdY[1]) / (i_lpdX[2] - i_lpdX[1])) / (i_lpdX[2] - i_lpdX[0]);
		double dFirst_Derivative = (i_lpdY[2] - i_lpdY[1])/(i_lpdX[2] - i_lpdX[1]) - dSecond_Derivative * 0.5 * (i_lpdX[2] - i_lpdX[1]);
		double dDelX = i_dX - i_lpdX[1];

		dY = i_lpdY[1] + (dFirst_Derivative + dSecond_Derivative * dDelX) * dDelX;
	}
	return dY;
}
void xspline_data::Clear_pointers(void)
{
	m_lpdX = 
	m_lpdY = 
	m_lpdFirst_Derivatives = 
	m_lpdSecond_Derivatites = NULL;
	m_uiNum_Points = 0;
	m_uiNum_Points_Allocated = 0;
}
xspline_data::xspline_data(const xspline_data &i_cRHO)
{
	Clear_pointers(); 
	Copy(i_cRHO);
}
void xspline_data::Allocate(unsigned int i_uiNum_Data_Points)
{
	if (i_uiNum_Data_Points > m_uiNum_Points_Allocated)
	{
		if (m_lpdX)
			delete [] m_lpdX;
		if (m_lpdY)
			delete [] m_lpdY;
		if (m_lpdFirst_Derivatives)
			delete [] m_lpdFirst_Derivatives;
		if (m_lpdSecond_Derivatites)
			delete [] m_lpdSecond_Derivatites;
		m_uiNum_Points_Allocated = i_uiNum_Data_Points;
		m_lpdX = new double[m_uiNum_Points_Allocated];
		m_lpdY = new double[m_uiNum_Points_Allocated];
		m_lpdFirst_Derivatives = new double[m_uiNum_Points_Allocated - 2];
		m_lpdSecond_Derivatites = new double[m_uiNum_Points_Allocated - 2];
	}
	m_uiNum_Points = i_uiNum_Data_Points;
}
void xspline_data::Copy(const xspline_data &i_cRHO)
{
	unsigned int uiSizeXY = m_uiNum_Points * sizeof(double);
	unsigned int uiSizeD = (m_uiNum_Points - 2) * sizeof(double);
	Allocate(i_cRHO.m_uiNum_Points);
	memcpy(m_lpdX,i_cRHO.m_lpdX,uiSizeXY);
	memcpy(m_lpdY,i_cRHO.m_lpdY,uiSizeXY);
	memcpy(m_lpdFirst_Derivatives,i_cRHO.m_lpdFirst_Derivatives,uiSizeD);
	memcpy(m_lpdSecond_Derivatites,i_cRHO.m_lpdSecond_Derivatites,uiSizeD);
}
void xspline_data::Initialize(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points)
{
	unsigned int uiI;
	Allocate(i_uiNum_Data_Points);
	unsigned int uiSizeXY = m_uiNum_Points * sizeof(double);
	memcpy(m_lpdX,i_lpdX,uiSizeXY);
	memcpy(m_lpdY,i_lpdY,uiSizeXY);
	Initialize_Derivatives();
}
void xspline_data::Initialize(const std::map<double, double> &i_mddData)
{
	unsigned int uiI = 0;
	Allocate(i_mddData.size());
	for (std::map<double, double>::const_iterator iterI = i_mddData.cbegin(); iterI != i_mddData.cend(); iterI++)
	{
		m_lpdX[uiI] = iterI->first;
		m_lpdY[uiI] = iterI->second;
		uiI++;
	}
	Initialize_Derivatives();
}
void xspline_data::Initialize(const std::vector<double> i_vdX, std::vector<double> &i_vdY)
{
	if (i_vdX.size() == i_vdY.size())
	{
		unsigned int uiI = 0;
		Allocate(i_vdX.size());
		for (uiI = 0; uiI < i_vdX.size(); uiI++)
		{
			m_lpdX[uiI] = i_vdX[uiI];
			m_lpdY[uiI] = i_vdY[uiI];
		}
		Initialize_Derivatives();
	}
	else
		fprintf(stderr,"XSPLINE: X and Y have different sizes in (std::vector<double>, std::vector<double>) form.\n");
}
void xspline_data::Initialize(const std::vector< std::pair<double, double> > &i_vdData)
{
	unsigned int uiI = 0;
	Allocate(i_vdData.size());
	for (std::vector< std::pair<double, double> >::const_iterator iterI = i_vdData.cbegin(); iterI != i_vdData.cend(); iterI++)
	{
		m_lpdX[uiI] = iterI->first;
		m_lpdY[uiI] = iterI->second;
		uiI++;
	}
	Initialize_Derivatives();
}

void xspline_data::Initialize_Derivatives(void)
{
	for (unsigned int uiI = 0; uiI < m_uiNum_Points - 2; uiI++)
	{
		m_lpdSecond_Derivatites[uiI] = 2.0 * ((m_lpdY[uiI + 2] - m_lpdY[uiI+1]) / (m_lpdX[uiI + 2] - m_lpdX[uiI+1]) - (m_lpdY[uiI] - m_lpdY[uiI+1]) / (m_lpdX[uiI] - m_lpdX[uiI+1])) / (m_lpdX[uiI+2] - m_lpdX[uiI]);
		m_lpdFirst_Derivatives[uiI] = (m_lpdY[uiI+2] - m_lpdY[uiI+1])/(m_lpdX[uiI+2] - m_lpdX[uiI+1]) - m_lpdSecond_Derivatites[uiI] * 0.5 * (m_lpdX[uiI+2] - m_lpdX[uiI+1]);
		if (std::isnan(m_lpdX[uiI]) || std::isinf(m_lpdX[uiI]))
			fprintf(stderr,"XSPLINE: X value is invalid at %i\n",uiI);
		if (std::isnan(m_lpdY[uiI]) || std::isinf(m_lpdY[uiI]))
			fprintf(stderr,"XSPLINE: Y value is invalid at %i\n",uiI);
		if (std::isnan(m_lpdFirst_Derivatives[uiI]) || std::isinf(m_lpdFirst_Derivatives[uiI]))
			fprintf(stderr,"XSPLINE: First derivative is invalid at %i\n",uiI);
		if (std::isnan(m_lpdSecond_Derivatites[uiI]) || std::isinf(m_lpdSecond_Derivatites[uiI]))
			fprintf(stderr,"XSPLINE: Second derivative is invalid at %i\n",uiI);
	}
}
double xspline_data::Interpolate(const double &i_dX) const
{
	double	dY = g_kcNAN.k_dNAN;
	if (m_uiNum_Points >= 3)
	{
		unsigned int uiLower, uiUpper;
		if (i_dX == m_lpdX[0])
		{
			dY = m_lpdY[0];
		}
		else if (i_dX == m_lpdX[m_uiNum_Points - 1])
		{
			dY = m_lpdY[m_uiNum_Points - 1];
		}
		else
		{
			XBracket(i_dX,m_lpdX,m_uiNum_Points,uiLower,uiUpper);
			if (uiLower != 0x0ffffffff && uiUpper != 0x0ffffffff)
			{
				unsigned int uiRefIdx = uiLower;
				if (uiUpper != m_uiNum_Points - 1)
					uiRefIdx = uiUpper;
				double dDelX = (i_dX - m_lpdX[uiRefIdx]);
				dY = m_lpdY[uiRefIdx] + (m_lpdFirst_Derivatives[uiRefIdx - 1] + 0.5 * m_lpdSecond_Derivatites[uiRefIdx - 1] * dDelX) * dDelX;
			}
			else
				fprintf(stderr,"XSPLINE: Unable to bracket %.2e.\n",i_dX);
		}
	}
	else
		fprintf(stderr,"XSPLINE: Not enough data to interpolate.\n");
	return dY;
}
xspline_data & xspline_data::operator =(const xspline_data &i_cRHO)
{
	Copy(i_cRHO);
	return *this;
}
