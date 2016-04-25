#include <string.h>
#include <xlinalg.h>

//----------------------------------------------------------------------------
//
// TRIDIAG_MATRIX class
// for a matrix of the form
// [b_1  c_1  0    0  ...  0 ]
// [a_2  b_2  c_2  0  ...  0 ]
// [0    a_3  b_3 c_3 ...  0 ]
// [...  ...  ... ... ... ...]
// [0 ... 0      a_(n-1) b_(n-1)]
//
//----------------------------------------------------------------------------
// Set size n of matrix
void	XTRIDIAG_MATRIX::Set_Size(unsigned int i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		if (m_lpdGH)
			delete [] m_lpdGH;
		m_uiN_Alloc = m_uiN = 0;
	}
	m_lpdValues = new double[i_uiN * 3];
	m_lpdGH = new double [i_uiN * 2];
	m_uiN_Alloc = m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(double) * m_uiN * 3);
}
// default constructor
XTRIDIAG_MATRIX::XTRIDIAG_MATRIX(void)
{
	m_lpdValues = NULL;
	m_lpdGH = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor to set size
XTRIDIAG_MATRIX::XTRIDIAG_MATRIX(unsigned int i_uiN)
{
	m_lpdValues = NULL;
	m_lpdGH = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Set_Size(i_uiN);
}
// copy constructor
XTRIDIAG_MATRIX::XTRIDIAG_MATRIX(const XTRIDIAG_MATRIX &i_cRHO)
{
	m_lpdValues = NULL;
	m_lpdGH = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Set_Size(i_cRHO.m_uiN);
	memcpy(m_lpdValues,i_cRHO.m_lpdValues,i_cRHO.m_uiN * 3 * sizeof(double));
}
// destructor
XTRIDIAG_MATRIX::~XTRIDIAG_MATRIX(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	m_lpdValues = NULL;
	if (m_lpdGH)
		delete [] m_lpdGH;
	m_lpdGH = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}

// set a particlar row/column
void	XTRIDIAG_MATRIX::Set(
	unsigned int i_uiRow, 
	unsigned int i_uiCol, 
	const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < 3)
	{
		unsigned int uiIdx = i_uiRow * 3 + i_uiCol;
		m_lpdValues[uiIdx] = 0.0;
		if (!(i_uiRow == 0 && i_uiCol == 0) && 
			!(i_uiRow == (m_uiN - 1) && i_uiCol == 2))
			m_lpdValues[uiIdx] = i_dValue;
	}
}

// set a row
void	XTRIDIAG_MATRIX::Set(
	unsigned int i_uiRow, 
	const double &i_dValue_Alpha, 
	const double &i_dValue_Beta, 
	const double &i_dValue_Gamma)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		unsigned int uiRow_Idx = i_uiRow * 3;
		m_lpdValues[uiRow_Idx + 0] = 
			m_lpdValues[uiRow_Idx + 1] = 
			m_lpdValues[uiRow_Idx + 2] = 0.0;
		if (i_uiRow >= 1)
			m_lpdValues[uiRow_Idx + 0] = i_dValue_Alpha;
		m_lpdValues[uiRow_Idx + 1] = i_dValue_Beta;
		if (i_uiRow < (m_uiN - 1))
			m_lpdValues[uiRow_Idx + 2] = i_dValue_Gamma;
	}
}
// add a value to a particular row/column element
void	XTRIDIAG_MATRIX::Element_Add(
	unsigned int i_uiRow, 
	unsigned int i_uiCol, 
	const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < 3)
	{
		if (!(i_uiRow == 0 && i_uiCol == 0) && 
			!(i_uiRow == (m_uiN - 1) && i_uiCol == 2))
			m_lpdValues[i_uiRow * 3 + i_uiCol] += i_dValue;
	}
}
// get a row/column element
double	XTRIDIAG_MATRIX::Get(unsigned int i_uiRow, unsigned int i_uiCol)
{
	double	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < 3)
		dRet =  m_lpdValues[i_uiRow * 3 + i_uiCol];
	return dRet;
}
// get g,h for a row
void	XTRIDIAG_MATRIX::Get_GH(unsigned int i_uiRow, double &o_dGi, double &o_dHi)
{
	o_dGi = 0.0;
	o_dHi = 0.0;
	if (m_lpdGH && i_uiRow < m_uiN)
	{
		o_dGi = m_lpdGH[i_uiRow * 2];
		o_dHi = m_lpdGH[i_uiRow * 2 + 1];
	}
}
// = operator (copy another tridiagonal matrix)
XTRIDIAG_MATRIX & XTRIDIAG_MATRIX::operator =(const XTRIDIAG_MATRIX &i_cRHO)
{
	if (m_uiN_Alloc < i_cRHO.m_uiN)
	{
		Set_Size(i_cRHO.m_uiN);
	}
	if (m_uiN_Alloc >= i_cRHO.m_uiN)
	{
		if (m_uiN_Alloc > i_cRHO.m_uiN)
			memset(m_lpdValues,0,sizeof(double) * m_uiN_Alloc * 3);
		memcpy(m_lpdValues,i_cRHO.m_lpdValues,sizeof(double) * i_cRHO.m_uiN * 3);
	}
	return *this;
}
// multiply a vector x into the matrix, such that r = Ax
XVECTOR XTRIDIAG_MATRIX::operator * (const XVECTOR &i_cRHO)
{
	XVECTOR	cRes(i_cRHO.m_uiN);

	if (i_cRHO.m_lpdValues && m_lpdValues && m_uiN == i_cRHO.m_uiN)
	{
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			cRes.m_lpdValues[uiRow] = 0.0;
			for (int iCol = -1; iCol <= 1; iCol++)
			{
				if (!(uiRow == 0 && iCol == -1) && 
					!(uiRow == (m_uiN - 1) && iCol == 1))
					cRes.m_lpdValues[uiRow] += 
							m_lpdValues[uiRow * 3 + iCol + 1] * 
							i_cRHO.m_lpdValues[uiRow + iCol];
			}
		}
	}
	return cRes;
}

// scale a row by a given value
void	XTRIDIAG_MATRIX::Scale_Row(unsigned int i_uiRow, const double &i_dScalar)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		unsigned int uiIdx_Ref = 3 * i_uiRow;
		for (unsigned int uiCol = 0; uiCol < 3; uiCol++)
		{
			m_lpdValues[uiIdx_Ref + uiCol] *= i_dScalar;

		}
	}
}
// For the expression Ax = b, compute x given b.
XVECTOR	XTRIDIAG_MATRIX::Compute_Inverse(const XVECTOR &i_vB)
{
	XVECTOR	vRes(i_vB.m_uiN);
	if (m_lpdValues && i_vB.m_lpdValues && i_vB.m_uiN == m_uiN)
	{
		m_lpdGH[(m_uiN - 2) * 2 + 0] = 
			-m_lpdValues[(m_uiN - 1) * 3 + 0] / 
			m_lpdValues[(m_uiN - 1) * 3 + 1]; // g_(N-1)
		m_lpdGH[(m_uiN - 2) * 2 + 1] = 
						i_vB.m_lpdValues[m_uiN - 1] / 
						m_lpdValues[(m_uiN - 1) * 3 + 1]; // h_(N-1)
		for (unsigned int uiI = m_uiN - 2; uiI > 0; uiI--)
		{
			double	dNumer = 1.0 / (m_lpdValues[uiI * 3 + 1] + 
						m_lpdValues[uiI * 3 + 2] * m_lpdGH[uiI * 2]);
			m_lpdGH[(uiI - 1) * 2 + 0] = -m_lpdValues[uiI * 3] * dNumer;
			m_lpdGH[(uiI - 1) * 2 + 1] = (i_vB.m_lpdValues[uiI] - 
						m_lpdValues[uiI * 3 + 2] * m_lpdGH[uiI * 2 + 1]) *
						dNumer;
		}
		vRes.m_lpdValues[0] = (i_vB.m_lpdValues[0] - m_lpdValues[2] * 
						m_lpdGH[1]) / (m_lpdValues[1] + m_lpdValues[2] * 
						m_lpdGH[0]);
		for (unsigned int uiI = 1; uiI < m_uiN; uiI++)
		{
			vRes.m_lpdValues[uiI] = m_lpdGH[(uiI - 1) * 2] * 
						vRes.m_lpdValues[uiI - 1] + m_lpdGH[uiI * 2 - 1];
		}
	}
	return vRes;
}



// print the matrix to file or console
void	XTRIDIAG_MATRIX::Print(FILE * fileOut)
{
	for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		double	dValue;
		for (unsigned int uiCol = 0; uiCol < m_uiN; uiCol++)
		{
			dValue = 0.0;
			int iDelta = uiCol - uiRow;

			if ((iDelta >= -1 && iDelta <= 1) && (iDelta != -1 || uiRow != 0))
			{
				dValue = m_lpdValues[uiRow * 3 + iDelta + 1];
			}
			if (fileOut)
				fprintf(fileOut,"%f\t",dValue);
			else
				printf("%f\t",dValue);
		}
		if (fileOut)
			fprintf(fileOut,"\n");
		else
			printf("\n");
	}
}


