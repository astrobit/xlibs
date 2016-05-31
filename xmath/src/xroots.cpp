#include "xroots.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <float.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

#ifndef _isnan
#define _isnan(a) std::isnan(a)
#endif

#ifndef _isinf
#define _isinf(a) std::isinf(a)
#endif

#ifndef _finite
#define _finite(a) std::isfinite(a)
#endif

#ifndef _min
#define _min(a,b) (a < b)?(a):(b)
#endif

#ifndef __min
#define __min(a,b) _min(a,b)
#endif

#ifndef _max
#define _max(a,b) (a > b)?(a):(b)
#endif

#ifndef __max
#define __max(a,b) _max(a,b)
#endif

////////////////////////////////////////////////////////////////////////////////////////
//
//	XROOTS.cpp
//
//	Source code for root fniding functions, as defined in xroots.h
//
//	Dependencies:
//		xcomplex.h
//		xpolynomial.h
//		math.h
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX g_cXROOT_Start_Point(1.0,-1.0);
XCOMPLEX g_cXROOT_Start_Point_Re(1.0,0.0);
XCOMPLEX g_cXROOT_Start_Point_Im(0.0,1.0);
unsigned int g_iXROOT_Max_Iterations = 120;
double	g_dXROOT_Threshold = DBL_EPSILON;
unsigned int g_iXROOT_Max_Used_Iter = 0;

////////////////////////////////////////////////////////////////////////////////////////
//
//	Set_Root_Found_Threshold
//
////////////////////////////////////////////////////////////////////////////////////////
void XROOT_Set_Root_Found_Threshold(const double &dThreshold)
{
	g_dXROOT_Threshold = dThreshold;
}

double	XROOT_Get_Root_Found_Threshold(void)
{
	return g_dXROOT_Threshold;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	Set_Max_Iter
//
////////////////////////////////////////////////////////////////////////////////////////
void XROOT_Set_Max_Iter(unsigned int uiMax)
{
	g_iXROOT_Max_Iterations = uiMax;
}

unsigned int XROOT_Get_Max_Iter(void)
{
	return g_iXROOT_Max_Iterations;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	Get_Max_Used_Iter
//
////////////////////////////////////////////////////////////////////////////////////////
unsigned int XROOT_Get_Max_Used_Iter(void)
{
	return g_iXROOT_Max_Used_Iter;
}
bool XROOT_Threshold_Test(const double &i_dX, const double &i_dDelta, unsigned int uiCriterion)
{
	int iXP_re;
	int iDeltaP_re;
	
	frexp(i_dX,&iXP_re);
	frexp(i_dDelta,&iDeltaP_re);

	return (i_dDelta == 0.0 || (i_dX != 0 && (unsigned int)abs(iXP_re - iDeltaP_re) >= uiCriterion) || (i_dX == 0.0 && (unsigned int)abs(iDeltaP_re) >= uiCriterion));
}
bool XROOT_Threshold_Test(const XCOMPLEX &cX, const XCOMPLEX &cDelta, unsigned int uiCriterion)
{
	return XROOT_Threshold_Test(cX.dRe,cDelta.dRe,uiCriterion) && XROOT_Threshold_Test(cX.dIm,cDelta.dIm,uiCriterion);
}

XPOLYNOMIAL XROOT_cPoly;
QFunctionD XROOT_funcEvalFunc;
QFunctionD XROOT_funcEvalFunc_Deriv;

XCOMPLEX XROOT_PolyInt_Eval(const XCOMPLEX &cX, const void * i_lpvData)
{
	return XROOT_cPoly.Eval(cX);
}
XCOMPLEX XROOT_PolyInt_Eval_Deriv(const XCOMPLEX &cX, const void * i_lpvData)
{
	return XROOT_cPoly.Eval(cX,1);
}

XCOMPLEX XROOT_FuncInt_Eval(const XCOMPLEX &cX, const void * i_lpvData)
{
	XCOMPLEX cRes(0.0,0.0);
	cRes.dRe = XROOT_funcEvalFunc(cX.dRe,i_lpvData);
	return cRes;
}
XCOMPLEX XROOT_FuncInt_Eval_Deriv(const XCOMPLEX &cX, const void * i_lpvData)
{
	XCOMPLEX cRes(0.0,0.0);
	cRes.dRe = XROOT_funcEvalFunc_Deriv(cX.dRe,i_lpvData);
	return cRes;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	Newton root finding algorithms
//
////////////////////////////////////////////////////////////////////////////////////////

bool			XROOT_Newton_Core(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC,XCOMPLEX & cRoot, const XCOMPLEX & cStart_Pt, const void * i_lpvData)
{
	XCOMPLEX		cRes_Func;
	XCOMPLEX		cRes_Deriv;
	XCOMPLEX		cX;
	XCOMPLEX		cXs = cStart_Pt;
	XCOMPLEX		cDelta;
	unsigned int	iCounter = 0;
	XCOMPLEX		cDenom;
	bool			bFound	=  false;

	g_iXROOT_Max_Used_Iter = 0;

	iCounter = 0;
	do
	{
		cX = cXs;

		cRes_Func = funcQFunctionC(cX,i_lpvData);
		cRes_Deriv = funcQFunctionDerivC(cX,i_lpvData);

		if (cRes_Deriv.dIm != 0.0 || cRes_Deriv.dRe != 0.0)
			cDelta = cRes_Func / cRes_Deriv;
		else
			cDelta = XCOMPLEX(1.0,1.0);
		cXs = cX - cDelta;
		iCounter++;
	}
	while (!_isnan(cX.dRe) && !_isnan(cX.dIm) && _finite(cX.dRe) && _finite(cX.dIm) && (cRes_Deriv.dIm != 0.0 || cRes_Deriv.dRe != 0.0) && !XROOT_Threshold_Test(cX,cDelta) && iCounter < g_iXROOT_Max_Iterations);
	if (XROOT_Threshold_Test(cX,cDelta))
	{
		bFound = true;
		cRoot = cXs;
		g_iXROOT_Max_Used_Iter = iCounter;
	}
	return bFound;
}



bool	XROOT_Newton(const XPOLYNOMIAL &cPoly,XCOMPLEX & cRoot)
{
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	return XROOT_Newton(XROOT_PolyInt_Eval,XROOT_PolyInt_Eval_Deriv,cRoot);
}

bool			XROOT_Newton(const XPOLYNOMIAL &cPoly,double & dRoot)
{
	XCOMPLEX cRoot;
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	dRoot = 0.0;
	bool bFound = XROOT_Newton(XROOT_PolyInt_Eval,XROOT_PolyInt_Eval_Deriv,cRoot);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}
bool			XROOT_Newton(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD,double & dRoot, const void * i_lpvData)
{
	XCOMPLEX cRoot;
	XROOT_funcEvalFunc = funcQFunctionD;
	XROOT_funcEvalFunc_Deriv = funcQFunctionDerivD;
	dRoot = 0.0;
	bool bFound = XROOT_Newton(XROOT_FuncInt_Eval,XROOT_FuncInt_Eval_Deriv,cRoot,i_lpvData);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}

bool			XROOT_Newton(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC,XCOMPLEX & cRoot, const void * i_lpvData)
{
	XCOMPLEX		cXs;
	int				iStart_Method = 0;
	bool			bFound	=  false;

	g_iXROOT_Max_Used_Iter = 0;
	iStart_Method = 0;
	do
	{
		switch (iStart_Method)
		{
		case 0:
			cXs = g_cXROOT_Start_Point;
			break;
		case 1:
			cXs = g_cXROOT_Start_Point_Im;
			break;
		case 2:
			cXs = g_cXROOT_Start_Point_Re;
			break;
		}
		bFound = XROOT_Newton_Core(funcQFunctionC,funcQFunctionDerivC,cRoot,cXs,i_lpvData);

		if (!bFound)
			iStart_Method++;
	}
	while (!bFound && iStart_Method < 3);
	return bFound;
}





bool	XROOT_Newton(const XPOLYNOMIAL &cPoly,XCOMPLEX & cRoot,const XCOMPLEX &cStart_Pt)
{
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	return XROOT_Newton(XROOT_PolyInt_Eval,XROOT_PolyInt_Eval_Deriv,cRoot,cStart_Pt);
}

bool			XROOT_Newton(const XPOLYNOMIAL &cPoly,double & dRoot,const double &dStart_Pt)
{
	XCOMPLEX cRoot;
	XCOMPLEX cStart(dStart_Pt,0.0);
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	dRoot = 0.0;
	bool bFound = XROOT_Newton(XROOT_PolyInt_Eval,XROOT_PolyInt_Eval_Deriv,cRoot,cStart);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}
bool			XROOT_Newton(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD,double & dRoot,const double &dStart_Pt, const void * i_lpvData)
{
	XCOMPLEX cRoot;
	XCOMPLEX cStart(dStart_Pt,0.0);
	XROOT_funcEvalFunc = funcQFunctionD;
	XROOT_funcEvalFunc_Deriv = funcQFunctionDerivD;
	dRoot = 0.0;
	bool bFound = XROOT_Newton(XROOT_FuncInt_Eval,XROOT_FuncInt_Eval_Deriv,cRoot,cStart,i_lpvData);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}

bool			XROOT_Newton(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC,XCOMPLEX & cRoot,const XCOMPLEX &cStart_Pt, const void * i_lpvData)
{
	g_iXROOT_Max_Used_Iter = 0;
	return XROOT_Newton_Core(funcQFunctionC,funcQFunctionDerivC,cRoot,cStart_Pt,i_lpvData);
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	Newton-Meahly root finding algorithms
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	Maehly_Root(XPOLYNOMIAL, XCOMPLEX)
////////////////////////////////////////////////////////////////////////////////////////
void XROOT_Maehly_Core(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const XCOMPLEX &cStart_Pt, const void * i_lpvData, bool bIsAngular)
{
	XCOMPLEX		cRes_Func;
	XCOMPLEX		cRes_Deriv;
	XCOMPLEX		cX;
	unsigned int	iIndex1;
	unsigned int	iIndex2;
	XCOMPLEX		cXs = cStart_Pt;
	XCOMPLEX		cStart_Pt_Lcl = cStart_Pt;
	XCOMPLEX		cRootSum;
	XCOMPLEX		cDelta;
	unsigned int	iCounter = 0;
	XCOMPLEX		cDenom;
	bool			bQuit = false;
	double			d2Pi = 2.0 * M_PI;

	g_iXROOT_Max_Used_Iter = 0;
	for (iIndex1 = 0; iRoots_Found < iMax_Results && !bQuit; iIndex1++)
	{
		iCounter = 0;
		do
		{
			cX = cXs;
			if (bIsAngular)
			{
				if (cX.dRe > d2Pi)
					cX.dRe = fmod(cX.dRe,d2Pi);
				else if (cX.dRe < 0.0)
					cX.dRe = fmod(cX.dRe,d2Pi) + d2Pi;
				if (cX.dIm> d2Pi)
					cX.dIm = fmod(cX.dIm,d2Pi);
				else if (cX.dIm < 0.0)
					cX.dIm = fmod(cX.dIm,d2Pi) + d2Pi;
			}

			cRootSum = XCOMPLEX(0,0);
			if (iRoots_Found >= 1)
			{
				for (iIndex2 = 0; iIndex2 <= iRoots_Found - 1 && !bQuit; iIndex2++)
				{
					if (cX == lpcResults[iIndex2])
						bQuit = true;
					else
						cRootSum += 1 / (cX - lpcResults[iIndex2]);
				}
			}

			cRes_Func = funcQFunctionC(cX,i_lpvData);
			if (cRes_Func.dRe == 0.0 && cRes_Func.dIm == 0.0)
				cDelta = XCOMPLEX(0.0,0.0);
			else
			{
				cRes_Deriv = funcQFunctionDerivC(cX,i_lpvData);

				cDenom = cRes_Deriv - cRes_Func * cRootSum;
				if (cDenom.abs() > g_dXROOT_Threshold)
				{
					cDelta = cRes_Func / cDenom;
					cXs = cX - cDelta;
					iCounter++;
				}
				else
				{
					// stop work 
					bQuit = true;
				}
			}
		}
		while (!bQuit && !XROOT_Threshold_Test(cX,cDelta) && iCounter < g_iXROOT_Max_Iterations);
		if (!bQuit && XROOT_Threshold_Test(cX,cDelta))
		{
			// run it through newton to refine the root
			if (bIsAngular)
			{
				if (cXs.dRe > d2Pi)
					cXs.dRe = fmod(cXs.dRe,d2Pi);
				else if (cXs.dRe < 0.0)
					cXs.dRe = fmod(cXs.dRe,d2Pi) + d2Pi;
				if (cXs.dIm> d2Pi)
					cXs.dIm = fmod(cXs.dIm,d2Pi);
				else if (cXs.dIm < 0.0)
					cXs.dIm = fmod(cXs.dIm,d2Pi) + d2Pi;
			}
			XCOMPLEX cNew_Root = cXs;
			XCOMPLEX cNew_RootTest;
			if (XROOT_Newton_Core(funcQFunctionC,funcQFunctionDerivC,cNew_RootTest,cXs,i_lpvData))
				cNew_Root = cNew_RootTest;

			{
				// confirm that this root hasn't already been found
				if (bIsAngular)
				{
					if (cNew_Root.dRe > d2Pi)
						cNew_Root.dRe = fmod(cNew_Root.dRe,d2Pi);
					else if (cNew_Root.dRe < 0.0)
						cNew_Root.dRe = fmod(cNew_Root.dRe,d2Pi) + d2Pi;
					if (cNew_Root.dIm> d2Pi)
						cNew_Root.dIm = fmod(cNew_Root.dIm,d2Pi);
					else if (cNew_Root.dIm < 0.0)
						cNew_Root.dIm = fmod(cNew_Root.dIm,d2Pi) + d2Pi;
				}
				bool bFound = false;
				for (unsigned int uiIndex = 0; uiIndex < iRoots_Found && !bFound; uiIndex++)
				{
					if (cNew_Root == lpcResults[iRoots_Found])
						bFound = true;
				}
				if (!bFound)
				{
					lpcResults[iRoots_Found] = cNew_Root;
					iRoots_Found++;
					if (iCounter > g_iXROOT_Max_Used_Iter)
						g_iXROOT_Max_Used_Iter = iCounter;
				}
				if (cXs == cStart_Pt_Lcl)
				{
					// randomize offset
					cStart_Pt_Lcl.dRe *= ((rand() / (double)RAND_MAX) + 1.0);
					cStart_Pt_Lcl.dIm *= ((rand() / (double)RAND_MAX) + 1.0);
				}

				cXs = cStart_Pt_Lcl;
			}
		}
		else
			bQuit = true;
	}
}
void XROOT_Maehly(const XPOLYNOMIAL &cPoly, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular)
{
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	unsigned int iMax_Results_Lcl = __min((unsigned int)cPoly.GetDegree(),iMax_Results);
	unsigned int uiTrivial_Zeros = 0;

	for (unsigned int uiIndex = 0; uiTrivial_Zeros < iMax_Results && uiIndex < (unsigned int)cPoly.GetDegree(); uiIndex++)
	{
		XCOMPLEX xcCoeff = cPoly.GetCoeffC(uiIndex);
		if (xcCoeff.dRe == 0.0 && xcCoeff.dIm == 0.0)
		{
			uiTrivial_Zeros++;
		}
	}
	XCOMPLEX *xcFound_Zeros = new XCOMPLEX[uiTrivial_Zeros];
	for (unsigned int uiIndex = 0; uiIndex < uiTrivial_Zeros; uiIndex++)
	{
		xcFound_Zeros[uiIndex] = XCOMPLEX(0.0,0.0);
	}
	XROOT_Maehly(XROOT_PolyInt_Eval,XROOT_PolyInt_Eval_Deriv,lpcResults, iMax_Results_Lcl, iRoots_Found, NULL, bIsAngular, xcFound_Zeros, uiTrivial_Zeros);
	delete xcFound_Zeros;
}
void XROOT_Maehly(const XPOLYNOMIAL &cPoly, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular)
{
	XCOMPLEX cRoot;
	XCOMPLEX * lpcResults = new XCOMPLEX[iMax_Results];
	unsigned int iRoots_Found_Lcl = 0;
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	memset(lpdResults,0,iMax_Results * sizeof(double));
	int iMax_Results_Lcl = __min((unsigned int)cPoly.GetDegree(),iMax_Results);
	unsigned int uiTrivial_Zeros = 0;

	for (unsigned int uiIndex = 0; uiTrivial_Zeros < iMax_Results && uiIndex < (unsigned int)cPoly.GetDegree(); uiIndex++)
	{
		XCOMPLEX xcCoeff = cPoly.GetCoeffC(uiIndex);
		if (xcCoeff.dRe == 0.0 && xcCoeff.dIm == 0.0)
		{
			uiTrivial_Zeros++;
		}
	}
	XCOMPLEX *xcFound_Zeros = new XCOMPLEX[uiTrivial_Zeros];
	for (unsigned int uiIndex = 0; uiIndex < uiTrivial_Zeros; uiIndex++)
	{
		xcFound_Zeros[uiIndex] = XCOMPLEX(0.0,0.0);
	}
	XROOT_Maehly(XROOT_PolyInt_Eval, XROOT_PolyInt_Eval_Deriv, lpcResults, iMax_Results_Lcl, iRoots_Found_Lcl, NULL, bIsAngular, xcFound_Zeros, uiTrivial_Zeros);
	delete xcFound_Zeros;
	unsigned int uiLcl_Idx = 0;
	for (unsigned int uiIndex = 0; uiIndex < iRoots_Found_Lcl && uiIndex < iMax_Results; uiIndex++)
	{
		if (lpcResults[uiIndex].dIm == 0.0)
		{
			lpdResults[uiLcl_Idx] = lpcResults[uiIndex].dRe;
			uiLcl_Idx++;
		}
	}
	iRoots_Found = uiLcl_Idx;
	delete lpcResults;
}
void XROOT_Maehly(QFunctionD funcQFunctionD, QFunctionD funcQFunctionDerivD, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData, bool bIsAngular)
{
	XROOT_funcEvalFunc = funcQFunctionD;
	XROOT_funcEvalFunc_Deriv = funcQFunctionDerivD;
	XCOMPLEX * lpcResults = new XCOMPLEX[iMax_Results];
	unsigned int iRoots_Found_Lcl = 0;
	memset(lpdResults,0,iMax_Results * sizeof(double));
	XROOT_Maehly(XROOT_FuncInt_Eval,XROOT_FuncInt_Eval_Deriv,lpcResults, iMax_Results, iRoots_Found_Lcl,i_lpvData,bIsAngular);
	unsigned int uiLcl_Idx = 0;
	for (unsigned int uiIndex = 0; uiIndex < iRoots_Found_Lcl && uiIndex < iMax_Results; uiIndex++)
	{
//		if (lpcResults[uiIndex].dIm == 0.0)
		{
			lpdResults[uiLcl_Idx] = lpcResults[uiIndex].dRe;
			uiLcl_Idx++;
		}
	}
	iRoots_Found = uiLcl_Idx;
	delete lpcResults;
}

////////////////////////////////////////////////////////////////////////////////////////
//	Maehly_Root(QFunctionC, XCOMPLEX)
////////////////////////////////////////////////////////////////////////////////////////
void XROOT_Maehly(QFunctionC funcQFunctionC, QFunctionC funcQFunctionDerivC, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, const void * i_lpvData, bool bIsAngular, XCOMPLEX * lpPrior_Zeros, unsigned int uiNum_Prior_Zeros)
{
	XCOMPLEX cXs;
	unsigned int iStart_Method = 0;

	memset(lpcResults,0,iMax_Results * sizeof(XCOMPLEX));
	iRoots_Found = 0;
	if (uiNum_Prior_Zeros != 0)
	{
		memcpy(lpcResults,lpPrior_Zeros,uiNum_Prior_Zeros * sizeof(XCOMPLEX));
		iRoots_Found += uiNum_Prior_Zeros;
	}

	g_iXROOT_Max_Used_Iter = 0;
	iStart_Method = 0;
	do
	{
		switch (iStart_Method)
		{
		case 0:
			cXs = g_cXROOT_Start_Point;
			break;
		case 1:
			cXs = g_cXROOT_Start_Point_Im;
			break;
		case 2:
			cXs = g_cXROOT_Start_Point_Re;
			break;
		}
		XROOT_Maehly_Core(funcQFunctionC,funcQFunctionDerivC,lpcResults,iMax_Results,iRoots_Found,cXs,i_lpvData,bIsAngular);
		iStart_Method++;
	}
	while (iStart_Method < 3 && iRoots_Found < iMax_Results);
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	Secant root finding algorithms
//
////////////////////////////////////////////////////////////////////////////////////////

bool XROOT_Secant_Core(QFunctionC funcQFunctionC, XCOMPLEX &cRoot, const XCOMPLEX &cStart_1, const XCOMPLEX &cStart_2, const void * i_lpvData)
{
	XCOMPLEX		cX;
	XCOMPLEX		cXs = cStart_1;
	XCOMPLEX		cX_1 = cStart_2;
	XCOMPLEX		cFx;
	XCOMPLEX		cFx_1;
	XCOMPLEX		cDelta;
	unsigned int	iCounter = 0;
	bool			bFound	=  false;

	g_iXROOT_Max_Used_Iter = 0;

	cFx_1 = funcQFunctionC(cX_1,i_lpvData);
	iCounter = 0;
	do
	{
		cX = cXs;
		cFx = funcQFunctionC(cX,i_lpvData);
		cDelta = (cX - cX_1) / (cFx - cFx_1) * cFx;
		cXs = cX - cDelta;
		cFx_1 = cFx;
		cX_1 = cX;
		iCounter++;
	}
	while (!XROOT_Threshold_Test(cX,cDelta) && iCounter < g_iXROOT_Max_Iterations);
	if (XROOT_Threshold_Test(cX,cDelta))
	{
		bFound = true;
		cRoot = cXs;
		g_iXROOT_Max_Used_Iter = iCounter;
	}
	return bFound;
}


bool XROOT_Secant(const XPOLYNOMIAL &cPoly,XCOMPLEX & cRoot)
{
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	return XROOT_Secant(XROOT_PolyInt_Eval,cRoot);
}

bool XROOT_Secant(const XPOLYNOMIAL &cPoly,double & dRoot)
{
	XCOMPLEX cRoot;
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	dRoot = 0.0;
	bool bFound = XROOT_Secant(XROOT_PolyInt_Eval,cRoot);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}
bool XROOT_Secant(QFunctionD funcQFunctionD, double & dRoot, const void * i_lpvData)
{
	XCOMPLEX cRoot;
	XROOT_funcEvalFunc = funcQFunctionD;
	dRoot = 0.0;
	bool bFound = XROOT_Secant(XROOT_FuncInt_Eval,cRoot,i_lpvData);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}


bool XROOT_Secant(QFunctionC funcQFunctionC, XCOMPLEX &cRoot, const void * i_lpvData)
{
	XCOMPLEX		cX;
	XCOMPLEX		cXs;
	XCOMPLEX		cX_1;
	XCOMPLEX		cFx;
	XCOMPLEX		cFx_1;
	int				iStart_Method = 0;
	bool			bFound	=  false;

	g_iXROOT_Max_Used_Iter = 0;
	iStart_Method = 0;
	do
	{
		switch (iStart_Method)
		{
		case 0:
			cXs = g_cXROOT_Start_Point;
			cX_1 = g_cXROOT_Start_Point * 1.1;
			break;
		case 1:
			cXs = g_cXROOT_Start_Point_Im;
			cX_1 = g_cXROOT_Start_Point_Im * 1.1;
			break;
		case 2:
			cXs = g_cXROOT_Start_Point_Re;
			cX_1 = g_cXROOT_Start_Point_Re * 1.1;
			break;
		}
		bFound = XROOT_Secant_Core(funcQFunctionC,cRoot,cXs,cX_1,i_lpvData);
		if (!bFound)
			iStart_Method++;
	}
	while (!bFound && iStart_Method < 3);
	return bFound;
}

bool		XROOT_Secant(const XPOLYNOMIAL &cPoly, XCOMPLEX &cRoot, const XCOMPLEX &cStart_Pt)
{
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	return XROOT_Secant(XROOT_PolyInt_Eval,cRoot,cStart_Pt);
}
bool		XROOT_Secant(const XPOLYNOMIAL &cPoly, double &dRoot, const double &dStart_Pt)
{
	XCOMPLEX cRoot;
	XCOMPLEX cStart(dStart_Pt,0.0);
	XROOT_cPoly = cPoly;
	XROOT_cPoly.Normalize();
	dRoot = 0.0;
	bool bFound = XROOT_Secant(XROOT_PolyInt_Eval,cRoot,cStart);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}
bool		XROOT_Secant(QFunctionD funcQFunctionD, double &dRoot, const double &dStart_Pt, const void * i_lpvData)
{
	XCOMPLEX cRoot;
	XCOMPLEX cStart(dStart_Pt,0.0);
	XROOT_funcEvalFunc = funcQFunctionD;
	dRoot = 0.0;
	bool bFound = XROOT_Secant(XROOT_FuncInt_Eval,cRoot,cStart,i_lpvData);
	if (bFound && cRoot.dIm == 0.0)
		dRoot = cRoot.dRe;
	else
		bFound = false;
	return bFound;
}
bool		XROOT_Secant(QFunctionC funcQFunctionC, XCOMPLEX &cRoot, const XCOMPLEX &cStart_Pt, const void * i_lpvData)
{
	XCOMPLEX cStart_1 = cStart_Pt * 1.1;
	return XROOT_Secant_Core(funcQFunctionC,cRoot,cStart_Pt,cStart_1,i_lpvData);
}


///////////////////////////////////////////////////////////////////////////////////////
void XROOT_Maehly_Secant(const XPOLYNOMIAL &cPoly, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found)
{
	XCOMPLEX cRes_Func;
	XCOMPLEX cRes_Deriv;
	XCOMPLEX cX;
	XCOMPLEX	cX_1;
	XCOMPLEX	cFx;
	XCOMPLEX	cFx_1;
	unsigned int	iIndex1;
	unsigned int iIndex2;
	XCOMPLEX cXs;
	XCOMPLEX cRootSum;
	double	dDelta;
	unsigned int		iCounter = 0;
	XCOMPLEX	cStart_Point = g_cXROOT_Start_Point_Re;
	unsigned int iStart_Method = 0;
	XCOMPLEX	cDenom;

	memset(lpcResults,0,iMax_Results * sizeof(XCOMPLEX));

	iRoots_Found = 0;
	g_iXROOT_Max_Used_Iter = 0;
	for (iIndex1 = 0; iIndex1 < iMax_Results; iIndex1++)
	{
		iStart_Method = 0;
		do
		{
			switch (iStart_Method)
			{
			case 2:
				cXs = g_cXROOT_Start_Point;
				cX_1 = XCOMPLEX(g_cXROOT_Start_Point.dRe,0.0);
				break;
			case 1:
				cXs = g_cXROOT_Start_Point_Im;
				cX_1 = g_cXROOT_Start_Point_Im * 1.1;
				break;
			case 0:
				cXs = g_cXROOT_Start_Point_Re;
				cX_1 = g_cXROOT_Start_Point_Re * 1.1;
				break;
			}

			cFx_1 = cPoly.Eval(cX_1);

			if (cFx_1.dIm == 0.0 && cFx_1.dRe == 0.0)
				cX_1 *= 1.1;
			iCounter = 0;
			do
			{
				cX = cXs;
				cRootSum = XCOMPLEX(0,0);
				if (iIndex1 >= 1)
				{
					for (iIndex2 = 0; iIndex2 <= iIndex1 - 1; iIndex2++)
						cRootSum += 1 / (cX - lpcResults[iIndex2]);
				}

				cRes_Func = cFx = cPoly.Eval(cX);
				cRes_Deriv = (cFx - cFx_1) / (cX - cX_1);
				cFx_1 = cFx;
				cX_1 = cX;

				cDenom = cRes_Deriv - cRes_Func * cRootSum;
				if (cDenom.abs() > g_dXROOT_Threshold)
				{
					cXs = cX - cRes_Func / cDenom;
					dDelta = cXs.abs() - cX.abs();
					iCounter++;
				}
				else
				{
					// stop work - found all roots
					iCounter = g_iXROOT_Max_Iterations + 1;
					dDelta = g_dXROOT_Threshold + 1.0;
					if (iStart_Method == 2)
					{
						iIndex2 = iIndex1;
						iIndex1 = iMax_Results;
					}
				}
			}
			while (fabs(dDelta) > g_dXROOT_Threshold && iCounter < g_iXROOT_Max_Iterations);
			if (fabs(dDelta) > g_dXROOT_Threshold)
				iStart_Method++;
		}
		while (fabs(dDelta) > g_dXROOT_Threshold && iStart_Method < 3);
		if (fabs(dDelta) < g_dXROOT_Threshold)
		{
			iRoots_Found++;
			lpcResults[iIndex1] = cXs;
			if (iCounter > g_iXROOT_Max_Used_Iter)
				g_iXROOT_Max_Used_Iter = iCounter;
		}
		if (iCounter >= g_iXROOT_Max_Iterations)
		{
			iIndex1 = iMax_Results;
		}
	}
}


XCOMPLEX		XROOT_NR_Newton(const XPOLYNOMIAL &cPoly, const XCOMPLEX &cX1, const XCOMPLEX &cX2, const double &dThresh)
{
	const int JMAX=20;
	int j;
	XCOMPLEX	df, dx, f, rtn;
	XCOMPLEX	cRef = 0.5 * (cX1 + cX2);
	double		dRadius = (cRef - cX1).abs();
	XCOMPLEX	cRet_Val(0.0,0.0);
	bool		bRoot_Found = false;
	bool		bError = false;
	rtn = cRef;	// initial guess
	for (j = 0; j < JMAX && !bRoot_Found && !bError; j++)
	{
		f = cPoly.Eval(rtn);
		df = cPoly.Eval(rtn,1);
		dx = f/df;
		rtn -= dx;
		if ((rtn - cRef).abs() < dRadius)
		{
			printf("Jumped out of brackets.");
			bError = true;
		}
		if (dx.abs() < dThresh)
		{
			bRoot_Found = true;
			cRet_Val = rtn;	// convergence
		}
	}
	if (!bRoot_Found && !bError)
		printf("Maximum number of iterations exceeded.");
	return cRet_Val;

}

bool			XROOT_NR_Newton_2(const XPOLYNOMIAL &cPoly, XCOMPLEX &cRoot)
{
	unsigned int		iCounter = 0;
	XCOMPLEX			cF;
	XCOMPLEX			cFp;
	XCOMPLEX			cXs;
	XCOMPLEX			cDelX;
	unsigned int iStart_Method = 0;
	bool				bRoot_Found = false;
	for (iStart_Method = 0; iStart_Method < 2 && !bRoot_Found; iStart_Method++)
	{
		switch (iStart_Method)
		{
		case 2:
			cXs = g_cXROOT_Start_Point;
			break;
		case 1:
			cXs = g_cXROOT_Start_Point_Im;
			break;
		case 0:
			cXs = g_cXROOT_Start_Point_Re;
			break;
		}
		for (iCounter = 0; iCounter < g_iXROOT_Max_Iterations && !bRoot_Found; iCounter++)
		{
			cF = cPoly.Eval(cXs);
			cFp = cPoly.Eval(cXs,1);
			cDelX = cF/cFp;
			cXs -= cDelX;
			if (cDelX.abs() < g_dXROOT_Threshold)
				bRoot_Found = true;	// convergence
		}
	}
	if (bRoot_Found)
		cRoot = cXs;
	return bRoot_Found; 

}



double	g_dXROOT_DK_Threshold = 2e-8;
unsigned int		g_uiXROOT_DK_Max_Iters = 60;
double XROOT_Get_Durand_Kerner_Threshold(void)
{
	return g_dXROOT_DK_Threshold;
}
void XROOT_Durand_Kerner(const XPOLYNOMIAL &xpolyPoly, XCOMPLEX * lpZeros, unsigned int uiMax_Zeros, unsigned int & uiNum_Zeros)
{
	unsigned int uiDegree = xpolyPoly.GetDegree();
	XCOMPLEX * lpResults = new XCOMPLEX[uiDegree];
	XCOMPLEX * lpDelta = new XCOMPLEX[uiDegree];
	XPOLYNOMIAL	xcpolyLcl = xpolyPoly;
	xcpolyLcl.Normalize();
	XCOMPLEX	xcMax_Delta(0,0);
	unsigned int uiMax_Iters = g_uiXROOT_DK_Max_Iters * uiDegree;

	for (unsigned int uiIndex = 0; uiIndex < uiDegree; uiIndex++)
	{
		lpResults[uiIndex] = XCOMPLEX(1.0 / (uiIndex+1),1.0 / (uiIndex+1));
		lpDelta[uiIndex] = XCOMPLEX(0.0,0.0);
	}

	bool bQuit = false;
	bool bContinue = true;
	unsigned int iCounter = 0;
	while (!bQuit && bContinue && iCounter < uiMax_Iters)
	{
		for (unsigned int uiIndex = 0; uiIndex < uiDegree; uiIndex++)
		{
			XCOMPLEX	xcDenom(1,0);
			for (unsigned int uiIndex2 = 0; uiIndex2 < uiDegree; uiIndex2++)
			{
				if (uiIndex2 != uiIndex)
				{
					xcDenom *= (lpResults[uiIndex] - lpResults[uiIndex2]);
				}
			}
			if (xcDenom.dRe != 0.0 || xcDenom.dIm != 0.0)
				lpDelta[uiIndex] = xcpolyLcl.Eval(lpResults[uiIndex]) / xcDenom;
			else
				bQuit = true;
		}
		iCounter++;
		bContinue = false;
		xcMax_Delta = XCOMPLEX(0,0);
		for (unsigned int uiIndex = 0; uiIndex < uiDegree; uiIndex++)
		{
			if (xcMax_Delta.modulus() < lpDelta[uiIndex].modulus())
				xcMax_Delta = lpDelta[uiIndex];

//			if (!XROOT_Threshold_Test(lpResults[uiIndex],lpDelta[uiIndex]))
//				bContinue = true;
		}
		if (xcMax_Delta.modulus() > 2e-8)
			bContinue = true;
		for (unsigned int uiIndex = 0; uiIndex < uiDegree; uiIndex++)
			lpResults[uiIndex] -= lpDelta[uiIndex];
	}

	uiNum_Zeros = 0;
	if (!bQuit)
	{
		for (unsigned int uiIndex = 0; uiIndex < uiMax_Zeros && uiIndex < uiDegree; uiIndex++)
		{
			XCOMPLEX xcRootRes;
			if (XROOT_Newton(xcpolyLcl,xcRootRes,lpResults[uiIndex]))
			{
				lpZeros[uiNum_Zeros] = xcRootRes;
				uiNum_Zeros++;
			}
			else if (!bContinue)
			{
				lpZeros[uiNum_Zeros] = lpResults[uiIndex];
				uiNum_Zeros++;
			}
		}
	}
}


bool XROOT_Muller_Core(QFunctionC funcQFunctionC, XCOMPLEX &xcResult, const void * i_lpvData, bool bIsAngular)
{
	bool bRoot_Found = false;
	XCOMPLEX	xcP[3] = {XCOMPLEX(0.0,0.0),XCOMPLEX(1.0,0.0),XCOMPLEX(3.0,0.0)};
	XCOMPLEX	xcDelta = XCOMPLEX(1.0,0.0); // results in p = {1,2,3} for first iteration - arbitrary
	unsigned int iCounter = 0;

	do {
		xcP[0] = xcP[1];
		xcP[1] = xcP[2];
		xcP[2] += xcDelta;
		XCOMPLEX	xcF0 = funcQFunctionC(xcP[0],i_lpvData);
		XCOMPLEX	xcF1 = funcQFunctionC(xcP[1],i_lpvData);
		XCOMPLEX	xcF2 = funcQFunctionC(xcP[2],i_lpvData);
		XCOMPLEX	xcP0_1 = xcP[0] - xcP[1];
		XCOMPLEX	xcP0_2 = xcP[0] - xcP[2];
		XCOMPLEX	xcP1_2 = xcP[1] - xcP[2];
		XCOMPLEX	xcF0_2 = xcF0 - xcF2;
		XCOMPLEX	xcF1_2 = xcF1 - xcF2;
		XCOMPLEX	xcDenom = 1.0 / (xcP0_2 * xcP1_2 * xcP0_1);

		XCOMPLEX	xcC = xcF2;
		XCOMPLEX	xcB = (xcP0_2 * xcP0_2 * xcF1_2 - xcP1_2 * xcP1_2 * xcF0_2) * xcDenom;
		XCOMPLEX	xcA = (xcP1_2 * xcF0_2 - xcP0_2 * xcF1_2) * xcDenom;
		XCOMPLEX	xcRadical = (xcB * xcB - 4 * xcA * xcC);

		xcDelta = - 2 * xcC / (xcB + XM_Sign(xcB.dRe) * xcRadical.sqrt());
		iCounter++;

	} while (!XROOT_Threshold_Test(xcP[2],xcDelta) && iCounter < g_iXROOT_Max_Iterations);
	if (iCounter < g_iXROOT_Max_Iterations)
	{
		bRoot_Found = true;
		xcResult = xcP[2] + xcDelta;
		if (bIsAngular)
		{
			XM_Angular_Normalize(xcResult.dRe);
			XM_Angular_Normalize(xcResult.dIm);
		}
	}
	return bRoot_Found;
}
bool XROOT_Muller(const XPOLYNOMIAL &cPoly, XCOMPLEX &xcResult, bool bIsAngular)
{
	XROOT_cPoly = cPoly;
	return XROOT_Muller_Core(XROOT_PolyInt_Eval,xcResult,NULL,bIsAngular);
}
bool			XROOT_Muller(const XPOLYNOMIAL &cPoly, double &dResult, bool bIsAngular)
{
	bool	bRet = false;
	XROOT_cPoly = cPoly;
	XCOMPLEX	xcRes;
	if (XROOT_Muller_Core(XROOT_PolyInt_Eval,xcRes,NULL,bIsAngular))
	{
		if (XROOT_Threshold_Test(XCOMPLEX(xcRes.dRe,0.0),XCOMPLEX(xcRes.dIm,0.0)) == 0.0)
		{
			bRet = true;
			dResult = xcRes.dRe;
		}
	}
	return bRet;
}
bool			XROOT_Muller(QFunctionC funcQFunctionC, XCOMPLEX &xcResult, const void * i_lpvData, bool bIsAngular)
{
	return XROOT_Muller_Core(funcQFunctionC,xcResult,i_lpvData,bIsAngular);
}
bool			XROOT_Muller(QFunctionD funcQFunctionD, double &dResult, const void * i_lpvData, bool bIsAngular)
{
	bool bRet = false;
	XCOMPLEX	xcRes;
	XROOT_funcEvalFunc = funcQFunctionD;
	if (XROOT_Muller_Core(XROOT_FuncInt_Eval,xcRes,i_lpvData,bIsAngular))
	{
		if (XROOT_Threshold_Test(XCOMPLEX(xcRes.dRe,0.0),XCOMPLEX(xcRes.dIm,0.0)) == 0.0)
		{
			bRet = true;
			dResult = xcRes.dRe;
		}
	}
	return bRet;
}

void			XROOT_Muller_Multi(const XPOLYNOMIAL &cPoly, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular)
{
	if (lpcResults && iMax_Results > 0)
	{
		bool bQuit = false;
		XCOMPLEX	xcResult;
		iRoots_Found = 0;
		XROOT_cPoly = cPoly;
		memset(lpcResults,0,iMax_Results);
		if (iMax_Results > (unsigned int)cPoly.GetDegree())
			iMax_Results = (unsigned int)cPoly.GetDegree();

		while (iRoots_Found < iMax_Results && iMax_Results && !bQuit)
		{
			if (XROOT_Muller_Core(XROOT_PolyInt_Eval,xcResult,NULL,bIsAngular))
			{
				lpcResults[iRoots_Found] = xcResult;
				XROOT_cPoly.Synthetic_Division(lpcResults[iRoots_Found]);
				iRoots_Found++;
			}
			else
				bQuit = true;
		}
	}
}
void XROOT_Muller_Multi(const XPOLYNOMIAL &cPoly, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular)
{
	if (lpdResults && iMax_Results > 0)
	{
		XPOLYNOMIAL	xcPoly_Lcl = cPoly;
		bool bQuit = false;
		double dResult;
		iRoots_Found = 0;
		XROOT_cPoly = cPoly;
		memset(lpdResults,0,iMax_Results);
		if (iMax_Results > (unsigned int)cPoly.GetDegree())
			iMax_Results = (unsigned int)cPoly.GetDegree();

		while (iRoots_Found < iMax_Results && iMax_Results && !bQuit)
		{
			if (XROOT_Muller(cPoly,dResult,bIsAngular))
			{
				lpdResults[iRoots_Found] = dResult;
				XROOT_cPoly.Synthetic_Division(XCOMPLEX(lpdResults[iRoots_Found],0.0));
				iRoots_Found++;
			}
			else
				bQuit = true;
		}
	}
}
/*
bool			XROOT_Laguerre(const XPOLYNOMIAL &cPoly, XCOMPLEX &xcResult, bool bIsAngular = false)
{
	XCOMPLEX	xcB;
	unsigned int uiDegree = cPoly.GetDegree();
	do
	{
		dABX = xcX.modulus();
		xcF = cPoly.Eval(xcX,2); // 2nd derivative
		xcD = cPoly.Eval(xcX,1); // 1st derivative
		xcB = cPoly.Eval(xcX);

		xcG = xcD / xcB;
		xcG2 = xcG * xcG;
		xcH = xcG2 - 2.0 * xcF / xcB;
		xcSQ = ((xcH * uiDegree - xcG2) * (uiDegree - 1)).sqrt();
		xcGp = xcG + xcSQ;
		xcGm = xcG - xcSQ;
		dABp = xcGp.modulus();
		dABm = xcGm.modulus();
		if (dABp < dABm)
		{
			xcGp = xcGm;
		}
		xcDX = __max(dABm,dABp) > 0.0 ? uiDegree / xcGp : 


		cRes_Func = funcQFunctionC(cX);
		cRes_Deriv = funcQFunctionDerivC(cX);

		if (cRes_Deriv.dIm != 0.0 || cRes_Deriv.dRe != 0.0)
			cDelta = cRes_Func / cRes_Deriv;
		else
			cDelta = XCOMPLEX(1.0,1.0);
		cXs = cX - cDelta;
		iCounter++;
	}
	while (!_isnan(cX.dRe) && !_isnan(cX.dIm) && _finite(cX.dRe) && _finite(cX.dIm) && (cRes_Deriv.dIm != 0.0 || cRes_Deriv.dRe != 0.0) && !XROOT_Threshold_Test(cX,cDelta) && iCounter < g_iXROOT_Max_Iterations);
}
bool			XROOT_Laguerre(const XPOLYNOMIAL &cPoly, double &dResult, bool bIsAngular = false);
void			XROOT_Laguerre_Multi(const XPOLYNOMIAL &cPoly, XCOMPLEX * lpcResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);
void			XROOT_Laguerre_Multi(const XPOLYNOMIAL &cPoly, double * lpdResults, unsigned int iMax_Results, unsigned int &iRoots_Found, bool bIsAngular = false);
*/
