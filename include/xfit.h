#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include <xpolynomial.h>
#include <xlinalg.h>
#include <xmath.h>

typedef double (*QFunctionDA)(const double &dX, const XVECTOR & i_vA);
typedef XVECTOR (*QFunctionVA)(const double &dX, const XVECTOR & i_vA, void * io_lpvData);

class XVECTOR_FNA
{
private:
	QFunctionDA *	m_lplpFunction;
	unsigned int	m_uiAllocated_Space;
	unsigned int	m_uiUsed_Space;
	void Copy(const XVECTOR_FNA & i_cRHO)
	{
		Allocate(i_cRHO.m_uiUsed_Space);

		for (unsigned int uiI = 0; uiI < m_uiUsed_Space; uiI++)
			m_lplpFunction[uiI] = i_cRHO.m_lplpFunction[uiI];
	}
public:
	XVECTOR_FNA(void)
	{
		m_lplpFunction = NULL;
		m_uiAllocated_Space = 0;
		m_uiUsed_Space = 0;
	}
	XVECTOR_FNA(const XVECTOR_FNA & i_cRHO)
	{
		m_lplpFunction = NULL;
		m_uiAllocated_Space = 0;
		m_uiUsed_Space = 0;
		Copy(i_cRHO);
	}
	XVECTOR_FNA & operator =(const XVECTOR_FNA & i_cRHO)
	{
		Copy(i_cRHO);
		return *this;
	}

	void Allocate(unsigned int i_uiSize)
	{
		if (m_uiAllocated_Space < i_uiSize)
		{
			if (m_lplpFunction)
				delete [] m_lplpFunction;
			m_uiAllocated_Space = 0;
			m_lplpFunction = new QFunctionDA[i_uiSize];
			if (m_lplpFunction)
				m_uiAllocated_Space = i_uiSize;
		}
		if (m_uiAllocated_Space > 0)
			m_uiUsed_Space = i_uiSize;
		else
			m_uiUsed_Space = m_uiAllocated_Space;
	}
	void Set(unsigned int i_uiIndex, QFunctionDA i_lpcFunction)
	{
		if (i_uiIndex < m_uiUsed_Space)
			m_lplpFunction[i_uiIndex] = i_lpcFunction;
	}
	QFunctionDA Get(unsigned int i_uiIndex) const
	{
		QFunctionDA fRet = NULL;
		if (i_uiIndex < m_uiUsed_Space)
			fRet =  m_lplpFunction[i_uiIndex];
		return fRet;
	}
	unsigned int Get_Size(void)
	{
		return m_uiUsed_Space;
	}
};


void PolynomialFit(const XVECTOR &i_cX, const XVECTOR &i_cY, const XVECTOR &i_cWeights, unsigned int i_uiDegree, XPOLYNOMIAL & o_polyFit, XSQUARE_MATRIX &o_cCovariance_Matrix);
bool GeneralFit(const XVECTOR &i_cX, const XVECTOR &i_cY,const XVECTOR &i_cW, QFunctionDA Fit_Function, XVECTOR_FNA i_vFit_Function_dA, XVECTOR & io_vFit_Parameters, XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dSmin, unsigned int i_uiMax_Iterations = 20, int i_iConvergence_Criterion = -30, unsigned int *o_lpiIterations = NULL);
bool GeneralFit(const XVECTOR &i_cX, const XVECTOR &i_cY,const XVECTOR &i_cW, QFunctionVA Fit_Function, XVECTOR & io_vFit_Parameters, XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dSmin, void * io_lpvData = NULL, unsigned int i_uiMax_Iterations = 20, int i_iConvergence_Criterion = -30, unsigned int *o_lpiIterations = NULL, XVECTOR * o_lpvEstimated_Fit_Parameters = NULL);

double	UsePolynomialFit(const double &i_dX, const XPOLYNOMIAL & o_polyFit, const XSQUARE_MATRIX &o_cCovariance_Matrix, double &o_dError);

void XFIT_Simplex(XVECTOR & io_vParameters, const XVECTOR & i_vParameters_Range_Start, const XVECTOR & i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, bool i_bQuiet = false, XVECTOR * i_lpvLower_Bounds = NULL, std::vector<bool> * i_lpvbLower_Bounds_Valid = NULL, XVECTOR * i_lpvUpper_Bounds = NULL, std::vector<bool> * i_lpvbUpper_Bounds_Valid = NULL);

