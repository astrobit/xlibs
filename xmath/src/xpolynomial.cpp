#include "xpolynomial.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

#ifndef _min
#define _min(a,b) (a < b)?(a):(b)
#endif

#ifndef _max
#define _max(a,b) (a > b)?(a):(b)
#endif


////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL.cpp
//
//	Source code for XPOLYNOMIAL, as defined in xpolynomial.h
//
//	Dependencies:
//		xpolynomial.h
//		xcomplex.h
//		float.h
//		math.h
//
////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Collapse_Degree
//
////////////////////////////////////////////////////////////////////////////////////////
void XPOLYNOMIAL::Collapse_Degree(void)
{
	while (m_lpCoefficients[m_iDegree].dRe == 0 && m_lpCoefficients[m_iDegree].dIm == 0.0 && m_iDegree >= 0)
		m_iDegree--;
	if (m_iDegree < 0)
		m_iDegree = 0;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Set_Allocated_Memory
//
////////////////////////////////////////////////////////////////////////////////////////

void XPOLYNOMIAL::Set_Allocated_Memory(int a_iDegree)
{
	if (m_iAllocated_Length < (a_iDegree + 1))
	{
		m_iAllocated_Length = (a_iDegree + 1);
		XCOMPLEX * lpTemp = (XCOMPLEX *)malloc(m_iAllocated_Length * sizeof(XCOMPLEX));
		memset(lpTemp,0,(m_iAllocated_Length * sizeof(XCOMPLEX)));
		if (m_lpCoefficients)
		{
			memcpy(lpTemp,m_lpCoefficients,(m_iDegree + 1) * sizeof(XCOMPLEX));
			free(m_lpCoefficients);
		}
		m_lpCoefficients = lpTemp;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Copy_Polynomial
//
////////////////////////////////////////////////////////////////////////////////////////

void XPOLYNOMIAL::Copy_Polynomial(const double * a_lpCoefficients, int a_iDegree)
{
	if (a_lpCoefficients)
	{
		memset(m_lpCoefficients,0,m_iAllocated_Length * sizeof(XCOMPLEX));
		for (int uiIndex = 0; uiIndex <= a_iDegree; uiIndex++)
		{
			m_lpCoefficients[uiIndex].dRe = a_lpCoefficients[uiIndex];
			m_lpCoefficients[uiIndex].dIm = 0.0;
		}

		m_iDegree = a_iDegree;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Copy_Polynomial
//
////////////////////////////////////////////////////////////////////////////////////////

void XPOLYNOMIAL::Copy_Polynomial(const XCOMPLEX * a_lpCoefficients, int a_iDegree)
{
	if (a_lpCoefficients)
	{
		memset(m_lpCoefficients,0,m_iAllocated_Length * sizeof(XCOMPLEX));
		memcpy(m_lpCoefficients,a_lpCoefficients,(a_iDegree + 1) * sizeof(XCOMPLEX));
		m_iDegree = a_iDegree;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	Constructors
//
////////////////////////////////////////////////////////////////////////////////////////

XPOLYNOMIAL::XPOLYNOMIAL(void)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(1);
}

XPOLYNOMIAL::XPOLYNOMIAL(const XPOLYNOMIAL &cRHO)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(cRHO.m_iDegree);
	Copy_Polynomial(cRHO.m_lpCoefficients,cRHO.m_iDegree);
}
XPOLYNOMIAL::XPOLYNOMIAL(const double * a_lpCoefficients, int a_iDegree)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(a_iDegree);
	if (a_lpCoefficients)
		Copy_Polynomial(a_lpCoefficients,a_iDegree);
}
XPOLYNOMIAL::XPOLYNOMIAL(const XCOMPLEX * a_lpCoefficients, int a_iDegree)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(a_iDegree);
	if (a_lpCoefficients)
		Copy_Polynomial(a_lpCoefficients,a_iDegree);
}
XPOLYNOMIAL::~XPOLYNOMIAL(void)
{
	m_iDegree = 0;
	m_iAllocated_Length = 0;
	free(m_lpCoefficients);
	m_lpCoefficients = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Set
//
////////////////////////////////////////////////////////////////////////////////////////

void XPOLYNOMIAL::Set(const double * a_lpCoefficients, int a_iDegree)
{
	XPOLYNOMIAL cTemp(a_lpCoefficients,a_iDegree);

	*this = cTemp;
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Set
//
////////////////////////////////////////////////////////////////////////////////////////

void XPOLYNOMIAL::Set(const XCOMPLEX * a_lpCoefficients, int a_iDegree)
{
	XPOLYNOMIAL cTemp(a_lpCoefficients,a_iDegree);

	*this = cTemp;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Set
//
////////////////////////////////////////////////////////////////////////////////////////
void XPOLYNOMIAL::SetCoefficient(const double &dCoefficient, int iDegree)
{
	if (iDegree >= 0 && iDegree <= m_iDegree)
	{
		m_lpCoefficients[iDegree].dRe = dCoefficient;
		m_lpCoefficients[iDegree].dIm = 0.0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Set
//
////////////////////////////////////////////////////////////////////////////////////////
void XPOLYNOMIAL::SetCoefficient(const XCOMPLEX &xcCoefficient, int iDegree)
{
	if (iDegree >= 0 && iDegree <= m_iDegree)
	{
		m_lpCoefficients[iDegree] = xcCoefficient;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	operators    (double)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator += (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator += (const double & dRHO)
{
	m_lpCoefficients[0].dRe += dRHO;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator -= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator -= (const double & dRHO)
{
	m_lpCoefficients[0].dRe -= dRHO;
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator *= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator *= (const double & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex].dRe *= dRHO;
		m_lpCoefficients[iIndex].dIm *= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator /= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator /= (const double & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex].dRe /= dRHO;
		m_lpCoefficients[iIndex].dIm /= dRHO;
	}
	return *this;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	operators    (XCOMPLEX)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator += (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator += (const XCOMPLEX & dRHO)
{
	m_lpCoefficients[0] += dRHO;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator -= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator -= (const XCOMPLEX & dRHO)
{
	m_lpCoefficients[0] -= dRHO;
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator *= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator *= (const XCOMPLEX & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex] *= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator /= (double)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator /= (const XCOMPLEX & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex] /= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	operators    (XPOLYNOMIAL)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator = (XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL &XPOLYNOMIAL::operator = (const XPOLYNOMIAL & cRHO)
{
	Set_Allocated_Memory(cRHO.m_iDegree);

	Copy_Polynomial(cRHO.m_lpCoefficients,cRHO.m_iDegree);

	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator += (XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator += (const XPOLYNOMIAL & cRHO)
{
	Set_Allocated_Memory(cRHO.m_iDegree);

	for (int iIndex = 0; iIndex <= cRHO.m_iDegree; iIndex++)
		m_lpCoefficients[iIndex] += cRHO.m_lpCoefficients[iIndex];

	if (cRHO.m_iDegree > m_iDegree)
		m_iDegree = cRHO.m_iDegree;

	Collapse_Degree();
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator -= (XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator -= (const XPOLYNOMIAL & cRHO)
{
	Set_Allocated_Memory(cRHO.m_iDegree);

	for (int iIndex = 0; iIndex <= cRHO.m_iDegree; iIndex++)
		m_lpCoefficients[iIndex] -= cRHO.m_lpCoefficients[iIndex];

	if (cRHO.m_iDegree > m_iDegree)
		m_iDegree = cRHO.m_iDegree;

	Collapse_Degree();

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::operator *= (XPOLYNOMIAL)
////////////////////////////////////////////////////////////////////////////////////////
XPOLYNOMIAL & XPOLYNOMIAL::operator *= (const XPOLYNOMIAL & cRHO)
{
	XCOMPLEX	*lpdTable_Out;
	XCOMPLEX	dMult_Curr;
	int iIndex;
	int iIndex2;
	bool	bUse_Out_Table = false;

	Set_Allocated_Memory(m_iDegree + cRHO.m_iDegree);

	if ((m_iDegree + cRHO.m_iDegree + 1) > m_iAllocated_Length)
		bUse_Out_Table = true;

	lpdTable_Out = (XCOMPLEX *)malloc((m_iDegree + cRHO.m_iDegree + 1) * sizeof(XCOMPLEX));
	if (lpdTable_Out)
	{
		for (iIndex = 0; iIndex <= (m_iDegree + cRHO.m_iDegree); iIndex++)
		{
			lpdTable_Out[iIndex] = XCOMPLEX(0.0,0.0);
			for (iIndex2 = 0; iIndex2 <= cRHO.m_iDegree; iIndex2++)
			{
				if ((iIndex - iIndex2) >= 0 && (iIndex - iIndex2) <= m_iDegree)
					dMult_Curr = m_lpCoefficients[iIndex - iIndex2];
				else
					dMult_Curr = XCOMPLEX(0.0,0.0);
				lpdTable_Out[iIndex] += (dMult_Curr * cRHO.m_lpCoefficients[iIndex2]);
			}
		}
		Copy_Polynomial(lpdTable_Out,m_iDegree + cRHO.m_iDegree);
		free(lpdTable_Out);
		lpdTable_Out = NULL;
		Collapse_Degree();

	}
	return *this;
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Eval
//
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::Eval(XCOMLPEX)
////////////////////////////////////////////////////////////////////////////////////////
XCOMPLEX XPOLYNOMIAL::Eval(const XCOMPLEX &cX, unsigned int iDeriv) const
{
	XCOMPLEX cRes(0.0,0.0);
	XCOMPLEX cTemp(1.0,0.0);
	double dDeriv = 1.0;
	for (int iIndex = iDeriv; iIndex <= m_iDegree; iIndex++)
	{
		if (iDeriv > 0)
		{
			dDeriv = iIndex;
			for (unsigned int iIndex2 = 1; iIndex2 < iDeriv; iIndex2++)
				dDeriv *= (iIndex - iIndex2);
		}
		cRes += m_lpCoefficients[iIndex] * cTemp * dDeriv;
		cTemp *= cX;
	}

	return cRes;
}
////////////////////////////////////////////////////////////////////////////////////////
//	XPOLYNOMIAL::Eval(double)
////////////////////////////////////////////////////////////////////////////////////////
double XPOLYNOMIAL::Eval(const double &dX, unsigned int iDeriv) const
{
	XCOMPLEX cX(dX,0.0);
	XCOMPLEX cRes = Eval(cX,iDeriv);
	return cRes.dRe;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Normalize
//
////////////////////////////////////////////////////////////////////////////////////////

XCOMPLEX XPOLYNOMIAL::Normalize(void)
{
	int iIndex;
	XCOMPLEX dDivisor = m_lpCoefficients[m_iDegree];
	if ((dDivisor.dRe != 0.0 || dDivisor.dIm != 0.0) && dDivisor.dRe != 1.0)
	{
		for (iIndex = 0; iIndex <= m_iDegree; iIndex++)
		{
			m_lpCoefficients[iIndex] /= dDivisor;
		}
	}
	return dDivisor;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::Get_Reduce_Coeff
//
////////////////////////////////////////////////////////////////////////////////////////

double XPOLYNOMIAL::Get_Reduce_Coeff(void) const
{
	int		iExp_Max = 0;
	int		iExp_Min = 1000;

	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		int iExp_Re;
		int iExp_Im;
		frexp(m_lpCoefficients[iIndex].dRe,&iExp_Re);
		frexp(m_lpCoefficients[iIndex].dIm,&iExp_Im);

		if (iExp_Re > iExp_Max)
			iExp_Max = iExp_Re;
		else if (iExp_Re < iExp_Min)
			iExp_Min = iExp_Re;

		if (iExp_Im > iExp_Max)
			iExp_Max = iExp_Im;
		else if (iExp_Im < iExp_Min)
			iExp_Min = iExp_Im;
	}
	int iExp_Mid = (iExp_Max + iExp_Min) / 2;
	double dRet = ldexp(1.0,iExp_Mid);
	return dRet;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	XPOLYNOMIAL::GetMaxCoeff
//
////////////////////////////////////////////////////////////////////////////////////////

double XPOLYNOMIAL::GetMaxCoeff(void) const
{
	double dRet = 0.0;
	double	dMan_Res = 0.0;
	int		iExp_Res = 0;
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		int iExp_Re;
		int iExp_Im;
		double dMan_Re;
		double dMan_Im;
		dMan_Re = frexp(m_lpCoefficients[iIndex].dRe,&iExp_Re);
		dMan_Im = frexp(m_lpCoefficients[iIndex].dIm,&iExp_Im);
		if (iExp_Im > iExp_Re)
		{
			if (iExp_Im > iExp_Res)
			{
				iExp_Res = iExp_Im;
				dMan_Res = dMan_Im;
			}
			else if (fabs(dMan_Im) > fabs(dMan_Res))
			{
				iExp_Res = iExp_Im;
				dMan_Res = dMan_Im;
			}
		}
		else
		{
			if (iExp_Re > iExp_Res)
			{
				iExp_Res = iExp_Re;
				dMan_Res = dMan_Re;
			}
			else if (fabs(dMan_Re) > fabs(dMan_Res))
			{
				iExp_Res = iExp_Re;
				dMan_Res = dMan_Re;
			}
		}
	}
	dRet = ldexp(dMan_Res,iExp_Res);
	return dRet;
}

unsigned int XPOLYNOMIAL::Get_Trivial_Zeros(void) const
{
	unsigned int uiCount = 0;
	for (unsigned int uiIndex = 0; uiIndex < (unsigned int)m_iDegree; uiIndex++)
	{
		if (m_lpCoefficients[uiIndex].dRe == 0.0 && m_lpCoefficients[uiIndex].dIm == 0.0)
			uiCount++;
		else
			uiIndex = m_iDegree;
	}
	return uiCount;
}

unsigned int XPOLYNOMIAL::Trivial_Divide(unsigned int uiDegree)
{
	unsigned int uiMaxDivide = Get_Trivial_Zeros();
	unsigned int uiRet = 0;
	unsigned int uiShift = uiDegree;
	if (uiDegree > uiMaxDivide)
	{
		uiRet = uiDegree - uiMaxDivide;
		uiShift = uiMaxDivide;
	}

	unsigned int uiNumToMove = (m_iDegree - uiShift + 1);
	memmove(m_lpCoefficients,&m_lpCoefficients[uiShift],(uiNumToMove * sizeof(XCOMPLEX)));
	m_iDegree -= uiShift;
	return uiRet;
}
void XPOLYNOMIAL::Trivial_Multiply(unsigned int uiDegree)
{
	if ((unsigned int)(m_iDegree + uiDegree) > (unsigned int)m_iAllocated_Length)
	{
		XCOMPLEX *lpNew = (XCOMPLEX *)malloc((m_iDegree + uiDegree + 1) * sizeof(XCOMPLEX));
		memset(lpNew,0,(uiDegree * sizeof(XCOMPLEX)));
		memcpy(&lpNew[uiDegree],m_lpCoefficients,(m_iDegree * sizeof(XCOMPLEX)));
		free(m_lpCoefficients);
		m_lpCoefficients = lpNew;
		m_iAllocated_Length = (m_iDegree + uiDegree + 1);
	}
	else
	{
		memmove(&m_lpCoefficients[uiDegree],m_lpCoefficients,(m_iDegree * sizeof(XCOMPLEX)));
		memset(m_lpCoefficients,0,(uiDegree * sizeof(XCOMPLEX)));
	}
	m_iDegree += uiDegree;
}

XCOMPLEX	XPOLYNOMIAL::Synthetic_Division(const XCOMPLEX & xcRoot)
{
	XCOMPLEX	xcRes = m_lpCoefficients[m_iDegree];
	if (m_iDegree > 0)
	{
		m_lpCoefficients[m_iDegree] = XCOMPLEX(0.0,0.0);
		for (unsigned int uiIndex = m_iDegree - 1; uiIndex < (unsigned int)m_iDegree; uiIndex--)
		{
			XCOMPLEX	xcRHO = xcRes * xcRoot;
			XCOMPLEX	xcTemp = m_lpCoefficients[uiIndex] + xcRHO;
			m_lpCoefficients[uiIndex] = xcRes;
			xcRes = xcTemp;
		}
		m_iDegree--;
	}
	return xcRes;
}


