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
//	xpolynomial.cpp
//
//	Source code for xpolynomial, as defined in xpolynomial.h
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
//	xpolynomial::Collapse_Degree
//
////////////////////////////////////////////////////////////////////////////////////////
void xpolynomial::Collapse_Degree(void)
{
	while (m_lpCoefficients[m_iDegree].dRe == 0 && m_lpCoefficients[m_iDegree].dIm == 0.0 && m_iDegree >= 0)
		m_iDegree--;
	if (m_iDegree < 0)
		m_iDegree = 0;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Set_Allocated_Memory
//
////////////////////////////////////////////////////////////////////////////////////////

void xpolynomial::Set_Allocated_Memory(int a_iDegree)
{
	if (m_iAllocated_Length < (a_iDegree + 1))
	{
		m_iAllocated_Length = (a_iDegree + 1);
		xcomplex * lpTemp = (xcomplex *)malloc(m_iAllocated_Length * sizeof(xcomplex));
		memset(lpTemp,0,(m_iAllocated_Length * sizeof(xcomplex)));
		if (m_lpCoefficients)
		{
			memcpy(lpTemp,m_lpCoefficients,(m_iDegree + 1) * sizeof(xcomplex));
			free(m_lpCoefficients);
		}
		m_lpCoefficients = lpTemp;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Copy_Polynomial
//
////////////////////////////////////////////////////////////////////////////////////////

void xpolynomial::Copy_Polynomial(const double * a_lpCoefficients, int a_iDegree)
{
	if (a_lpCoefficients)
	{
		memset(m_lpCoefficients,0,m_iAllocated_Length * sizeof(xcomplex));
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
//	xpolynomial::Copy_Polynomial
//
////////////////////////////////////////////////////////////////////////////////////////

void xpolynomial::Copy_Polynomial(const xcomplex * a_lpCoefficients, int a_iDegree)
{
	if (a_lpCoefficients)
	{
		memset(m_lpCoefficients,0,m_iAllocated_Length * sizeof(xcomplex));
		memcpy(m_lpCoefficients,a_lpCoefficients,(a_iDegree + 1) * sizeof(xcomplex));
		m_iDegree = a_iDegree;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	Constructors
//
////////////////////////////////////////////////////////////////////////////////////////

xpolynomial::xpolynomial(void)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(1);
}

xpolynomial::xpolynomial(const xpolynomial &cRHO)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(cRHO.m_iDegree);
	Copy_Polynomial(cRHO.m_lpCoefficients,cRHO.m_iDegree);
}
xpolynomial::xpolynomial(const double * a_lpCoefficients, int a_iDegree)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(a_iDegree);
	if (a_lpCoefficients)
		Copy_Polynomial(a_lpCoefficients,a_iDegree);
}
xpolynomial::xpolynomial(const xcomplex * a_lpCoefficients, int a_iDegree)
{
	m_iDegree = 0;
	m_lpCoefficients = NULL;
	m_iAllocated_Length = 0;
	Set_Allocated_Memory(a_iDegree);
	if (a_lpCoefficients)
		Copy_Polynomial(a_lpCoefficients,a_iDegree);
}
xpolynomial::~xpolynomial(void)
{
	m_iDegree = 0;
	m_iAllocated_Length = 0;
	free(m_lpCoefficients);
	m_lpCoefficients = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Set
//
////////////////////////////////////////////////////////////////////////////////////////

void xpolynomial::Set(const double * a_lpCoefficients, int a_iDegree)
{
	xpolynomial cTemp(a_lpCoefficients,a_iDegree);

	*this = cTemp;
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Set
//
////////////////////////////////////////////////////////////////////////////////////////

void xpolynomial::Set(const xcomplex * a_lpCoefficients, int a_iDegree)
{
	xpolynomial cTemp(a_lpCoefficients,a_iDegree);

	*this = cTemp;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Set
//
////////////////////////////////////////////////////////////////////////////////////////
void xpolynomial::SetCoefficient(const double &dCoefficient, int iDegree)
{
	if (iDegree >= 0 && iDegree <= m_iDegree)
	{
		m_lpCoefficients[iDegree].dRe = dCoefficient;
		m_lpCoefficients[iDegree].dIm = 0.0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Set
//
////////////////////////////////////////////////////////////////////////////////////////
void xpolynomial::SetCoefficient(const xcomplex &xcCoefficient, int iDegree)
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
//	xpolynomial::operator += (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator += (const double & dRHO)
{
	m_lpCoefficients[0].dRe += dRHO;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator -= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator -= (const double & dRHO)
{
	m_lpCoefficients[0].dRe -= dRHO;
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator *= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator *= (const double & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex].dRe *= dRHO;
		m_lpCoefficients[iIndex].dIm *= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator /= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator /= (const double & dRHO)
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
//	operators    (xcomplex)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator += (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator += (const xcomplex & dRHO)
{
	m_lpCoefficients[0] += dRHO;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator -= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator -= (const xcomplex & dRHO)
{
	m_lpCoefficients[0] -= dRHO;
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator *= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator *= (const xcomplex & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex] *= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator /= (double)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator /= (const xcomplex & dRHO)
{
	for (int iIndex = 0; iIndex <= m_iDegree; iIndex++)
	{
		m_lpCoefficients[iIndex] /= dRHO;
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//
//	operators    (xpolynomial)
//
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator = (xpolynomial)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial &xpolynomial::operator = (const xpolynomial & cRHO)
{
	Set_Allocated_Memory(cRHO.m_iDegree);

	Copy_Polynomial(cRHO.m_lpCoefficients,cRHO.m_iDegree);

	return *this;
}
////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::operator += (xpolynomial)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator += (const xpolynomial & cRHO)
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
//	xpolynomial::operator -= (xpolynomial)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator -= (const xpolynomial & cRHO)
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
//	xpolynomial::operator *= (xpolynomial)
////////////////////////////////////////////////////////////////////////////////////////
xpolynomial & xpolynomial::operator *= (const xpolynomial & cRHO)
{
	xcomplex	*lpdTable_Out;
	xcomplex	dMult_Curr;
	int iIndex;
	int iIndex2;
	bool	bUse_Out_Table = false;

	Set_Allocated_Memory(m_iDegree + cRHO.m_iDegree);

	if ((m_iDegree + cRHO.m_iDegree + 1) > m_iAllocated_Length)
		bUse_Out_Table = true;

	lpdTable_Out = (xcomplex *)malloc((m_iDegree + cRHO.m_iDegree + 1) * sizeof(xcomplex));
	if (lpdTable_Out)
	{
		for (iIndex = 0; iIndex <= (m_iDegree + cRHO.m_iDegree); iIndex++)
		{
			lpdTable_Out[iIndex] = xcomplex(0.0,0.0);
			for (iIndex2 = 0; iIndex2 <= cRHO.m_iDegree; iIndex2++)
			{
				if ((iIndex - iIndex2) >= 0 && (iIndex - iIndex2) <= m_iDegree)
					dMult_Curr = m_lpCoefficients[iIndex - iIndex2];
				else
					dMult_Curr = xcomplex(0.0,0.0);
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
//	xpolynomial::Eval
//
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//	xpolynomial::Eval(XCOMLPEX)
////////////////////////////////////////////////////////////////////////////////////////
xcomplex xpolynomial::Eval(const xcomplex &cX, unsigned int iDeriv) const
{
	xcomplex cRes(0.0,0.0);
	xcomplex cTemp(1.0,0.0);
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
//	xpolynomial::Eval(double)
////////////////////////////////////////////////////////////////////////////////////////
double xpolynomial::Eval(const double &dX, unsigned int iDeriv) const
{
	xcomplex cX(dX,0.0);
	xcomplex cRes = Eval(cX,iDeriv);
	return cRes.dRe;
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	xpolynomial::Normalize
//
////////////////////////////////////////////////////////////////////////////////////////

xcomplex xpolynomial::Normalize(void)
{
	int iIndex;
	xcomplex dDivisor = m_lpCoefficients[m_iDegree];
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
//	xpolynomial::Get_Reduce_Coeff
//
////////////////////////////////////////////////////////////////////////////////////////

double xpolynomial::Get_Reduce_Coeff(void) const
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
//	xpolynomial::GetMaxCoeff
//
////////////////////////////////////////////////////////////////////////////////////////

double xpolynomial::GetMaxCoeff(void) const
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

unsigned int xpolynomial::Get_Trivial_Zeros(void) const
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

unsigned int xpolynomial::Trivial_Divide(unsigned int uiDegree)
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
	memmove(m_lpCoefficients,&m_lpCoefficients[uiShift],(uiNumToMove * sizeof(xcomplex)));
	m_iDegree -= uiShift;
	return uiRet;
}
void xpolynomial::Trivial_Multiply(unsigned int uiDegree)
{
	if ((unsigned int)(m_iDegree + uiDegree) > (unsigned int)m_iAllocated_Length)
	{
		xcomplex *lpNew = (xcomplex *)malloc((m_iDegree + uiDegree + 1) * sizeof(xcomplex));
		memset(lpNew,0,(uiDegree * sizeof(xcomplex)));
		memcpy(&lpNew[uiDegree],m_lpCoefficients,(m_iDegree * sizeof(xcomplex)));
		free(m_lpCoefficients);
		m_lpCoefficients = lpNew;
		m_iAllocated_Length = (m_iDegree + uiDegree + 1);
	}
	else
	{
		memmove(&m_lpCoefficients[uiDegree],m_lpCoefficients,(m_iDegree * sizeof(xcomplex)));
		memset(m_lpCoefficients,0,(uiDegree * sizeof(xcomplex)));
	}
	m_iDegree += uiDegree;
}

xcomplex	xpolynomial::Synthetic_Division(const xcomplex & xcRoot)
{
	xcomplex	xcRes = m_lpCoefficients[m_iDegree];
	if (m_iDegree > 0)
	{
		m_lpCoefficients[m_iDegree] = xcomplex(0.0,0.0);
		for (unsigned int uiIndex = m_iDegree - 1; uiIndex < (unsigned int)m_iDegree; uiIndex--)
		{
			xcomplex	xcRHO = xcRes * xcRoot;
			xcomplex	xcTemp = m_lpCoefficients[uiIndex] + xcRHO;
			m_lpCoefficients[uiIndex] = xcRes;
			xcRes = xcTemp;
		}
		m_iDegree--;
	}
	return xcRes;
}


