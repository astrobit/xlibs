#include "xmath.h"
#include "xcomplex.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cmath>

XMATH_CONSTANTS	g_cConstants;

XMATH_CONSTANTS::XMATH_CONSTANTS(void)
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

inline bool XMATH_Delta_Threshold_Test(const XCOMPLEX &cX, const XCOMPLEX &cDelta)
{
	return (XMATH_Delta_Threshold_Test(cX.dRe,cDelta.dRe) && XMATH_Delta_Threshold_Test(cX.dIm,cDelta.dIm));
}
inline bool XMATH_Threshold_Test(const XCOMPLEX &xcX1, const XCOMPLEX &xcX2, const double &dThreshold_Pct)
{
	double	dMod_X1 = xcX1.modulus();
	double	dDelta_Mod = (xcX2 - xcX1).modulus();
	double	dErr = dDelta_Mod / dMod_X1 * 100.0;
	return (dErr < dThreshold_Pct);
}

XCOMPLEX XM_Simpsons_Integration(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, unsigned int uiNumSteps, const void * i_lpvData)
{
	XCOMPLEX	xcStep = (xcEnd - xcStart) / uiNumSteps;
	XCOMPLEX	xcIntegral(0.0,0.0);

	
	xcIntegral = lpfFn(xcStart,i_lpvData) + lpfFn(xcEnd,i_lpvData);
	for (unsigned int uiIndex = 1; uiIndex < uiNumSteps; uiIndex++)
	{
		XCOMPLEX	xcX = xcStart + xcStep * uiIndex;
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
	for (unsigned int uiIndex = 1; uiIndex < uiNumSteps; uiIndex++)
	{
		double	dX = dStart + dStep * uiIndex;
		dIntegral += ((uiIndex & 1) * 2 + 2) * lpfFn(dX,i_lpvData);
	}
	dIntegral *= dStep / 3.0;

	return dIntegral;
}

XCOMPLEX	g_XMNR_Trapezoid_Res(0.0,0.0);
XCOMPLEX XMNR_Trapezoid(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, unsigned int uiN, const void * i_lpvData)
{
	XCOMPLEX	xcDelta = (xcEnd - xcStart);
	if (uiN == 1)
		g_XMNR_Trapezoid_Res = 0.5 * xcDelta * (lpfFn(xcStart,i_lpvData) + lpfFn(xcEnd,i_lpvData));
	else
	{
		unsigned int uiIT = 1, uiJ;
		for (uiJ = 1; uiJ < uiN - 1; uiJ++) uiIT <<= 1;
		XCOMPLEX xcDel = xcDelta / uiIT;
		XCOMPLEX xcX0 = xcStart + 0.5 * xcDel;
		XCOMPLEX xcSum = XCOMPLEX(0.0,0.0);
		for (uiJ = 0; uiJ < uiIT; uiJ++) xcSum += lpfFn(xcDel * uiJ + xcX0,i_lpvData);
		g_XMNR_Trapezoid_Res = 0.5 *(g_XMNR_Trapezoid_Res + xcDelta * xcSum / uiIT);
	}
	return g_XMNR_Trapezoid_Res;
}
XCOMPLEX XMNR_Q_Trapezoid(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	XCOMPLEX xcS_Last;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ < JMAX && !bQuit; uiJ++)
	{
		XCOMPLEX xcS = XMNR_Trapezoid(lpfFn,xcStart,xcEnd,uiJ,i_lpvData);
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
		xcS_Last = XCOMPLEX(0.0,0.0);
	return xcS_Last;
}

XCOMPLEX XM_Simpsons_Integration_Fast(QFunctionC lpfFn, const XCOMPLEX &xcStart, const XCOMPLEX &xcEnd, const void * i_lpvData)
{
	const unsigned int JMAX = 32;
	XCOMPLEX	xcSt_Last(0.0,0.0);
	XCOMPLEX	xcS_Last(0.0,0.0);
	XCOMPLEX	xcS;
	bool bQuit = false;
	for (unsigned int uiJ = 1; uiJ <= JMAX && !bQuit; uiJ++)
	{
		XCOMPLEX	xcSt = XMNR_Trapezoid(lpfFn,xcStart,xcEnd,uiJ, i_lpvData);
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
		xcS_Last = XCOMPLEX(0.0,0.0);
	return xcS_Last;
}


double	XM_Runge_Kutte(const XCOMPLEX &xcX, QFunctionC lpfFn, const double &dStep, const void * i_lpvData)
{
	double	dRes;
	XCOMPLEX	cF = lpfFn( xcX,i_lpvData ).sqrt();
	double		dPhi1 = cF.argument();
	XCOMPLEX	xcX2 = xcX + XCOMPLEX(cos(dPhi1),sin(-dPhi1)) * dStep * 0.5;
	XCOMPLEX	cF2 = lpfFn( xcX2,i_lpvData ).sqrt();
	double		dPhi2 = cF2.argument();
	XCOMPLEX	xcX3 = xcX + XCOMPLEX(cos(dPhi2),sin(-dPhi2)) * dStep * 0.5;
	XCOMPLEX	cF3 = lpfFn( xcX3,i_lpvData ).sqrt();
	double		dPhi3 = cF3.argument();
	XCOMPLEX	xcX4 = xcX + XCOMPLEX(cos(dPhi3),sin(-dPhi3)) * dStep;
	XCOMPLEX	cF4 = lpfFn( xcX4,i_lpvData ).sqrt();
	double		dPhi4 = cF4.argument();
	dRes = -(dPhi1 + 2.0 * (dPhi2 + dPhi3) + dPhi4) / 6.0;

	return dRes;
}
double	XM_Runge_Kutte_Init(const XCOMPLEX &xcX, QFunctionC lpfFn, const double &dPhi1, const double &dStep, const void * i_lpvData)
{
	double	dRes;
	XCOMPLEX	xcX2 = xcX + XCOMPLEX(cos(dPhi1),sin(-dPhi1)) * dStep * 0.5;
	XCOMPLEX	cF2 = lpfFn( xcX2,i_lpvData ).sqrt();
	double		dPhi2 = cF2.argument();
	XCOMPLEX	xcX3 = xcX + XCOMPLEX(cos(dPhi2),sin(-dPhi2)) * dStep * 0.5;
	XCOMPLEX	cF3 = lpfFn( xcX3,i_lpvData ).sqrt();
	double		dPhi3 = cF3.argument();
	XCOMPLEX	xcX4 = xcX + XCOMPLEX(cos(dPhi3),sin(-dPhi3)) * dStep;
	XCOMPLEX	cF4 = lpfFn( xcX4,i_lpvData ).sqrt();
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
XSPLINE_DATA::XSPLINE_DATA(void)
{
	m_lpdX = 
	m_lpdY = 
	m_lpdFirst_Derivatives = 
	m_lpdSecond_Derivatites = NULL;
	m_uiNum_Points = 0;
	m_uiNum_Points_Allocated = 0;
}
XSPLINE_DATA::XSPLINE_DATA(const XSPLINE_DATA &i_cRHO)
{
	m_lpdX = 
	m_lpdY = 
	m_lpdFirst_Derivatives = 
	m_lpdSecond_Derivatites = NULL;
	m_uiNum_Points = 0;
	m_uiNum_Points_Allocated = 0;
	Copy(i_cRHO);
}
void XSPLINE_DATA::Allocate(unsigned int i_uiNum_Data_Points)
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
void XSPLINE_DATA::Copy(const XSPLINE_DATA &i_cRHO)
{
	unsigned int uiSizeXY = m_uiNum_Points * sizeof(double);
	unsigned int uiSizeD = (m_uiNum_Points - 2) * sizeof(double);
	Allocate(i_cRHO.m_uiNum_Points);
	memcpy(m_lpdX,i_cRHO.m_lpdX,uiSizeXY);
	memcpy(m_lpdY,i_cRHO.m_lpdY,uiSizeXY);
	memcpy(m_lpdFirst_Derivatives,i_cRHO.m_lpdFirst_Derivatives,uiSizeD);
	memcpy(m_lpdSecond_Derivatites,i_cRHO.m_lpdSecond_Derivatites,uiSizeD);
}
void XSPLINE_DATA::Initialize(const double * i_lpdX, const double * i_lpdY, unsigned int i_uiNum_Data_Points)
{
	unsigned int uiI;
	Allocate(i_uiNum_Data_Points);
	unsigned int uiSizeXY = m_uiNum_Points * sizeof(double);
	memcpy(m_lpdX,i_lpdX,uiSizeXY);
	memcpy(m_lpdY,i_lpdY,uiSizeXY);
	for (uiI = 0; uiI < i_uiNum_Data_Points - 2; uiI++)
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
double XSPLINE_DATA::Interpolate(const double &i_dX) const
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
XSPLINE_DATA & XSPLINE_DATA::operator =(const XSPLINE_DATA &i_cRHO)
{
	Copy(i_cRHO);
	return *this;
}
