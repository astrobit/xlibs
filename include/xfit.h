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

typedef double (*QFunctionDA)(const double &dX, const xvector & i_vA);
typedef xvector (*QFunctionVA)(const double &dX, const xvector & i_vA, void * io_lpvData);

class xvector_FNA
{
private:
	QFunctionDA *	m_lplpFunction;
	unsigned int	m_uiAllocated_Space;
	unsigned int	m_uiUsed_Space;
	void Copy(const xvector_FNA & i_cRHO)
	{
		Allocate(i_cRHO.m_uiUsed_Space);

		for (unsigned int uiI = 0; uiI < m_uiUsed_Space; uiI++)
			m_lplpFunction[uiI] = i_cRHO.m_lplpFunction[uiI];
	}
public:
	xvector_FNA(void)
	{
		m_lplpFunction = NULL;
		m_uiAllocated_Space = 0;
		m_uiUsed_Space = 0;
	}
	xvector_FNA(const xvector_FNA & i_cRHO)
	{
		m_lplpFunction = NULL;
		m_uiAllocated_Space = 0;
		m_uiUsed_Space = 0;
		Copy(i_cRHO);
	}
	xvector_FNA & operator =(const xvector_FNA & i_cRHO)
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
typedef xvector_FNA XVECTOR_FNA;

void PolynomialFit(const xvector &i_cX, const xvector &i_cY, const xvector &i_cWeights, unsigned int i_uiDegree, xpolynomial & o_polyFit, xsquare_matrix &o_cCovariance_Matrix);
bool GeneralFit(const xvector &i_cX, const xvector &i_cY,const xvector &i_cW, QFunctionDA Fit_Function, xvector_FNA i_vFit_Function_dA, xvector & io_vFit_Parameters, xsquare_matrix &o_cCovariance_Matrix, double &o_dSmin, unsigned int i_uiMax_Iterations = 20, int i_iConvergence_Criterion = -30, unsigned int *o_lpiIterations = NULL);
bool GeneralFit(const xvector &i_cX, const xvector &i_cY,const xvector &i_cW, QFunctionVA Fit_Function, xvector & io_vFit_Parameters, xsquare_matrix &o_cCovariance_Matrix, double &o_dSmin, void * io_lpvData = NULL, unsigned int i_uiMax_Iterations = 20, int i_iConvergence_Criterion = -30, unsigned int *o_lpiIterations = NULL, xvector * o_lpvEstimated_Fit_Parameters = NULL);

double	UsePolynomialFit(const double &i_dX, const xpolynomial & o_polyFit, const xsquare_matrix &o_cCovariance_Matrix, double &o_dError);

void XFIT_Simplex(xvector & io_vParameters, const xvector & i_vParameters_Range_Start, const xvector & i_vEpsilon, QFunctionV i_lpfvChi_Squared_Function, void * i_lpvData, bool i_bQuiet = false, xvector * i_lpvLower_Bounds = NULL, std::vector<bool> * i_lpvbLower_Bounds_Valid = NULL, xvector * i_lpvUpper_Bounds = NULL, std::vector<bool> * i_lpvbUpper_Bounds_Valid = NULL);

