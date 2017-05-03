#include <string.h>
#include <xlinalg.h>


//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
// default constructor
xsquare_matrix::xsquare_matrix(void)
{
	m_lpdValues = NULL;
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = NULL;
	m_lpuiPivot_Table = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor sith specified size
xsquare_matrix::xsquare_matrix(unsigned int i_uiN)
{
	m_lpdValues = new double[i_uiN * i_uiN];
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = NULL;
	m_lpuiPivot_Table = NULL;
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xsquare_matrix::xsquare_matrix(const xsquare_matrix &i_cRHO)
{
	m_lpdValues = new double[i_cRHO.m_uiN * i_cRHO.m_uiN];
	m_lpuiPivot_Table = NULL;
	m_uiN = i_cRHO.m_uiN;
	m_uiN_Alloc = i_cRHO.m_uiN;
	memcpy(m_lpdValues,i_cRHO.m_lpdValues,
		i_cRHO.m_uiN * i_cRHO.m_uiN * sizeof(double));
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = NULL;
	Perform_LU_Decomposition();
}
// desructor
xsquare_matrix::~xsquare_matrix(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	if (m_lpuiPivot_Table)
		delete [] m_lpuiPivot_Table;
	if (m_lpdL_Values)
		delete [] m_lpdL_Values;
	if (m_lpdU_Values)
		delete [] m_lpdU_Values;
	m_lpdValues = NULL;
	m_lpuiPivot_Table = NULL;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = NULL;
}
// set size of matrix
void	xsquare_matrix::Set_Size(unsigned int i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpdValues = new double[i_uiN * i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(double) * m_uiN * m_uiN);
}
// set matrix element
void	xsquare_matrix::Set(
			unsigned int i_uiRow, 
			unsigned int i_uiCol, 
			const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
	{
		m_bLU_Initialized &= (m_lpdValues[i_uiRow * m_uiN + i_uiCol] == i_dValue); // 
		m_lpdValues[i_uiRow * m_uiN + i_uiCol] = i_dValue;
	}
}
// set column vector
void	xsquare_matrix::Set(unsigned int i_uiCol, const XVECTOR &i_vCol)
{
	if (m_lpdValues && i_uiCol < m_uiN && 
		i_vCol.m_lpdValues && i_vCol.m_uiN == m_uiN)
	{
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			m_bLU_Initialized &= (m_lpdValues[uiRow * m_uiN + i_uiCol] == i_vCol.m_lpdValues[uiRow]); // 
			m_lpdValues[uiRow * m_uiN + i_uiCol] = 
				i_vCol.m_lpdValues[uiRow];
		}
	}
}
// add a value to an element
void	xsquare_matrix::Element_Add(
		unsigned int i_uiRow, 
		unsigned int i_uiCol, 
		const double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
	{
		m_bLU_Initialized &= (i_dValue == 0.0); // 
		m_lpdValues[i_uiRow * m_uiN + i_uiCol] += i_dValue;
	}
}
// retrieve an element value
double	xsquare_matrix::Get(unsigned int i_uiRow, unsigned int i_uiCol) const
{
	double	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
		dRet =  m_lpdValues[i_uiRow * m_uiN + i_uiCol];
	return dRet;
}
XVECTOR	xsquare_matrix::GetColumn(unsigned int i_uiCol) const
{
	XVECTOR	vRet;
	unsigned int uiRow;
	if (m_lpdValues && i_uiCol < m_uiN)
	{
		vRet.Set_Size(m_uiN);
		for (uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			vRet.m_lpdValues[uiRow] = m_lpdValues[uiRow * m_uiN + i_uiCol];
		}
	}
	return vRet;
}
// = operator: copy matrix
xsquare_matrix & xsquare_matrix::operator =(const xsquare_matrix &i_cRHO)
{
	m_bLU_Initialized = false;
	if (m_uiN_Alloc < i_cRHO.m_uiN)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		if (m_lpdL_Values)
			delete [] m_lpdL_Values;
		if (m_lpdU_Values)
			delete [] m_lpdU_Values;
		m_lpdL_Values = m_lpdU_Values = NULL;
		m_lpdValues = new double[i_cRHO.m_uiN * i_cRHO.m_uiN];
		m_uiN = m_uiN_Alloc = i_cRHO.m_uiN;
	}
	if (m_uiN_Alloc >= i_cRHO.m_uiN)
	{
		if (m_uiN_Alloc > i_cRHO.m_uiN)
			memset(m_lpdValues,0,m_uiN_Alloc * m_uiN_Alloc * sizeof(double));
		memcpy(m_lpdValues,i_cRHO.m_lpdValues,
			sizeof(double) * i_cRHO.m_uiN * i_cRHO.m_uiN);
	}
	return *this;
}
// matrix multiplication
xsquare_matrix & xsquare_matrix::operator *=(const xsquare_matrix &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN && m_uiN > 0)
	{
		m_bLU_Initialized = false;
		double * lpdI = new double[m_uiN * m_uiN];
		memset(lpdI,0,sizeof(double) * m_uiN * m_uiN); // clear
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
			for (unsigned int uiCol = 0; uiCol < m_uiN; uiCol++)
				for (unsigned int uiK = 0; uiK < m_uiN; uiK++)
					lpdI[uiRow * m_uiN + uiCol] += 
						m_lpdValues[uiRow * m_uiN + uiK] * 
							i_cRHO.m_lpdValues[uiK * m_uiN + uiCol];
		memcpy(m_lpdValues,lpdI,sizeof(double) * m_uiN * m_uiN);
		delete [] lpdI;
	}
	return *this;
}
// matrix multiplication
xsquare_matrix xsquare_matrix::operator *(const xsquare_matrix &i_cRHO) const
{
	xsquare_matrix cMatrix(*this);
	cMatrix *= i_cRHO;
	return cMatrix;
}
// vector multiplication: matrix A, vector x:  r = Ax
XVECTOR xsquare_matrix::operator * (const XVECTOR &i_cRHO) const
{
	XVECTOR	cRes(i_cRHO.m_uiN);

	if (i_cRHO.m_lpdValues && m_lpdValues && m_uiN == i_cRHO.m_uiN)
	{
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			cRes.m_lpdValues[uiRow] = 0.0;
			for (unsigned int uiCol = 0; uiCol < m_uiN; uiCol++)
			{
				cRes.m_lpdValues[uiRow] += 
					m_lpdValues[uiRow * m_uiN + uiCol] * 
						i_cRHO.m_lpdValues[uiCol];
			}
		}
	}
	return cRes;
}

// scalar multiplication
xsquare_matrix & xsquare_matrix::operator *=(const double &i_dRHO)
{
	if (m_lpdValues)
	{
		unsigned int uiNN = m_uiN * m_uiN;
		for (unsigned int uiIdx = 0; uiIdx < uiNN; uiIdx++)
		{
			m_lpdValues[uiIdx] *= i_dRHO;
		}
	}
	return *this;
}
// scalar multiplication
xsquare_matrix xsquare_matrix::operator *(const double &i_dRHO)  const
{
	xsquare_matrix cRet(*this);
	cRet *= i_dRHO;
	return cRet;
}
// scalar multiplication
xsquare_matrix operator * (const double &i_dLHO, const xsquare_matrix &i_vRHO)
{
	return i_vRHO * i_dLHO;
}
	// matrix addition
xsquare_matrix & xsquare_matrix::operator +=(const xsquare_matrix &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN)
	{
		unsigned int uiNum = m_uiN * m_uiN;
		for (unsigned int uiI = 0; uiI < uiNum; uiI++)
		{
			m_lpdValues[uiI] += i_cRHO.m_lpdValues[uiI];
		}
		m_bLU_Initialized = false;
	}
	return *this;
}
xsquare_matrix xsquare_matrix::operator +(const xsquare_matrix &i_cRHO) const
{
	xsquare_matrix cMatrixOut = *this;
	cMatrixOut += i_cRHO;
	return cMatrixOut;
}
xsquare_matrix & xsquare_matrix::operator -=(const xsquare_matrix &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN)
	{
		unsigned int uiNum = m_uiN * m_uiN;
		for (unsigned int uiI = 0; uiI < uiNum; uiI++)
		{
			m_lpdValues[uiI] -= i_cRHO.m_lpdValues[uiI];
		}
		m_bLU_Initialized = false;
	}
	return *this;
}
xsquare_matrix xsquare_matrix::operator -(const xsquare_matrix &i_cRHO)  const
{
	xsquare_matrix cMatrixOut = *this;
	cMatrixOut -= i_cRHO;
	return cMatrixOut;
}

// set the matrix to the idenity matrix
void	xsquare_matrix::Identity(void)
{
	if (m_lpdValues)
	{
		Zero();
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			m_lpdValues[uiRow * m_uiN + uiRow] = 1.0;
		}
	}
}
// set all elements to zero
void	xsquare_matrix::Zero(void)
{
	if (m_lpdValues)
	{
		m_bLU_Initialized = false;
		if (m_uiN != m_uiN_Alloc)
		{
			fprintf(stderr,"N != nalloc in sqaure matrix\n");
			fflush(stderr);
		}
		memset(m_lpdValues,0,m_uiN_Alloc * m_uiN_Alloc * sizeof(double));
	}
}

// scale an entire matrix row by a value
void	xsquare_matrix::Scale_Row(unsigned int i_uiRow, const double &i_dScalar)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_bLU_Initialized &= (i_dScalar == 1.0);
		unsigned int uiIdx_Ref = m_uiN * i_uiRow;
		for (unsigned int uiCol = 0; uiCol < m_uiN; uiCol++)
		{
			m_lpdValues[uiIdx_Ref + uiCol] *= i_dScalar;

		}
	}
}
// add two rows of the matrix together, leaving the result in the 'left 
// hand operand' row.  A scalar factor is applied to the 'right hand
// operand' row before the addition.  If the Assume RR flag is true,
// only columns > right hand operand are modified. (the matrix is
// assumed to be 0 in the RHO row to the left of this column.
void	xsquare_matrix::Add_Rows(
			unsigned int i_uiRow_LHO, 
			unsigned int i_uiRow_RHO, 
			const double &i_dRHO_Scalar, 
			bool i_bAssume_RR) 
{
	unsigned int uiStart_Idx = 0;
	// assume all columns to the right of the diagonal are 0
	if (i_bAssume_RR) 
		uiStart_Idx = i_uiRow_RHO;

	unsigned int uiIdx_Ref_L = i_uiRow_LHO * m_uiN;
	unsigned int uiIdx_Ref_R = i_uiRow_RHO * m_uiN;
	for (unsigned int uiCol = uiStart_Idx; uiCol < m_uiN; uiCol++)
	{
		m_bLU_Initialized &= (i_dRHO_Scalar == 0.0 || m_lpdValues[uiIdx_Ref_R + uiCol] == 0.0);
		m_lpdValues[uiIdx_Ref_L + uiCol] += i_dRHO_Scalar * 
								m_lpdValues[uiIdx_Ref_R + uiCol];
	}
}

// Swap two rows
void	xsquare_matrix::Swap_Rows(unsigned int i_uiRow_1, unsigned int i_uiRow_2)
{
	if (m_lpdValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		m_bLU_Initialized = false;
		unsigned int uiSize = sizeof(double) * m_uiN;
		double * lpdTemp = new double [m_uiN];
		memcpy(lpdTemp,&m_lpdValues[i_uiRow_1 * m_uiN],uiSize);
		memcpy(&m_lpdValues[i_uiRow_1 * m_uiN],
					&m_lpdValues[i_uiRow_2 * m_uiN],uiSize);
		memcpy(&m_lpdValues[i_uiRow_2 * m_uiN],lpdTemp,uiSize);
		delete [] lpdTemp;
	}
}

// perform Guass-Jordanian elimination:  The matrix will become
// it's inverse
void	xsquare_matrix::Inverse_GJ(void)
{
	if (m_lpdValues)
	{
		xsquare_matrix	cI(m_uiN);
		cI.Identity();
		// allocate work space
		double	dScalar;

		// Using row reduction, reduce the source matrix to the identity, 
		// and the identity will become A^{-1}
		for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			if (m_lpdValues[uiRow * m_uiN + uiRow] == 0.0)
			{
				for (unsigned int uiRowInner = uiRow + 1; 
						uiRowInner < m_uiN; 
						uiRowInner++)
				{
					if (m_lpdValues[uiRowInner * m_uiN + uiRow] != 0.0)
					{
						Swap_Rows(uiRow,uiRowInner);
						cI.Swap_Rows(uiRow,uiRowInner);
					}
				}
			}
			dScalar = 1.0 / m_lpdValues[uiRow * m_uiN + uiRow];
			Scale_Row(uiRow,dScalar);
			cI.Scale_Row(uiRow,dScalar);

			for (unsigned int uiRowInner = 0; 
					uiRowInner < m_uiN; 
					uiRowInner++)
			{
				double	dRow_Scalar = 
							-m_lpdValues[uiRowInner * m_uiN + uiRow];
				if (uiRowInner != uiRow)
				{
					cI.Add_Rows(uiRowInner,uiRow,dRow_Scalar,false);
					Add_Rows(uiRowInner,uiRow,dRow_Scalar,true);
				}
			}
		}
		// copy result into current matrix
		*this = cI;
	}
}

XVECTOR	xsquare_matrix::Back_Substituion(const double * lpdValue_set, const XVECTOR &i_vB)
{
	XVECTOR vX(m_uiN);
	unsigned int uiRow;
	unsigned int uiCol;
	// Assuming we have an upper triangular matrix
	// perform back substitution
	for (uiRow = m_uiN - 1; uiRow < m_uiN; uiRow--)
	{
		vX.m_lpdValues[uiRow] = i_vB.m_lpdValues[uiRow];
		for (uiCol = uiRow + 1; uiCol < m_uiN; uiCol++)
		{
			vX.m_lpdValues[uiRow] -= 
				lpdValue_set[uiRow * m_uiN + uiCol] * 
				vX.m_lpdValues[uiCol];
		}
		vX.m_lpdValues[uiRow] /= lpdValue_set[uiRow * (1 + m_uiN)];
	}
	return vX;
}

XVECTOR	xsquare_matrix::Forward_Substituion(const XVECTOR &i_vB)
{
	XVECTOR vX(m_uiN);
	unsigned int uiRow;
	unsigned int uiCol;
	if (!m_bLU_Initialized)
		Perform_LU_Decomposition();
	if (m_bLU_Initialized)
	{
		// Assuming we have a lower triangular matrix
		// perform forward substitution
		for (uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			vX.m_lpdValues[uiRow] = i_vB.m_lpdValues[uiRow];
			for (uiCol = 0; uiCol < uiRow; uiCol++)
			{
				vX.m_lpdValues[uiRow] -= 
					m_lpdL_Values[uiRow * m_uiN + uiCol] * 
					vX.m_lpdValues[uiCol];
			}
			vX.m_lpdValues[uiRow] /= m_lpdL_Values[uiRow * (1 + m_uiN)];
		}
	}
	return vX;
}

// Perform Gaussian elimination with back substituion, given vector b,
// find vector X such that Ax = b
XVECTOR	xsquare_matrix::Solve_GEb(const XVECTOR &i_vB)
{
	XVECTOR	vX;
	XVECTOR	vBlcl = i_vB;
	if (m_lpdValues && i_vB.m_lpdValues && i_vB.m_uiN == m_uiN)
	{
		double * lpdValues_Store = new double [m_uiN * m_uiN];
		// store original matrix
		memcpy(lpdValues_Store,m_lpdValues,
					sizeof(double) * m_uiN * m_uiN);
		vX.Set_Size(m_uiN);

		unsigned int uiRef_Idx = 0;
		double	dMax = 0.0;
		unsigned int uiRow, uiRowInner, uiCol;
		for (uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			// find max valued row
			uiRef_Idx = uiRow;
			for (uiRowInner = uiRow; uiRowInner < m_uiN; uiRowInner++)
			{
				if (m_lpdValues[uiRowInner * m_uiN + uiRow] > dMax)
				{
					uiRef_Idx = uiRowInner;
					dMax = m_lpdValues[uiRowInner * m_uiN + uiRow];
				}
			}
			Swap_Rows(uiRow,uiRef_Idx);
			vBlcl.Swap_Rows(uiRow,uiRef_Idx);
			// Perform Gaussian elimination for this column
			for (uiRowInner = uiRow + 1; uiRowInner < m_uiN; uiRowInner++)
			{
				double	dInner_Scalar = 
							-m_lpdValues[uiRowInner * m_uiN + uiRow] / 
									m_lpdValues[uiRow * m_uiN + uiRow];
				for (uiCol = uiRow; uiCol < m_uiN; uiCol++)
				{
					m_lpdValues[uiRowInner * m_uiN + uiCol] += 
						m_lpdValues[uiRow * m_uiN + uiCol] * 
						dInner_Scalar;
				}
				vBlcl.m_lpdValues[uiRowInner] += 
						vBlcl.m_lpdValues[uiRow] * dInner_Scalar;
			}
		}
		// Perform backsubstituion step
		vX = Back_Substituion(m_lpdValues,vBlcl);
		// restore original matrix
		memcpy(m_lpdValues,lpdValues_Store,
					sizeof(double) * m_uiN * m_uiN);
		delete [] lpdValues_Store;
	}
	return vX;
}
void xsquare_matrix::Perform_LU_Decomposition(void)
{// Perform LU Decomposition using Crout's method
	unsigned int uiRow, uiCol, uiK;
	if (!m_bLU_Initialized && m_lpdValues)
	{
		if (!m_lpdL_Values)
			m_lpdL_Values = new double [m_uiN_Alloc * m_uiN_Alloc];
		if (!m_lpdU_Values)
			m_lpdU_Values = new double [m_uiN_Alloc * m_uiN_Alloc];
		if (!m_lpuiPivot_Table)
			m_lpuiPivot_Table = new unsigned int [m_uiN_Alloc];
		for (uiRow = 0; uiRow < m_uiN_Alloc; uiRow++)
			m_lpuiPivot_Table[uiRow] = uiRow;

		memset(m_lpdU_Values,0,sizeof(double) * m_uiN * m_uiN);
		memset(m_lpdL_Values,0,sizeof(double) * m_uiN * m_uiN);
		m_lpdL_Values[0] = 1.0;
		for (uiCol = 0; uiCol < m_uiN; uiCol++)
		{
			m_lpdU_Values[uiCol] = m_lpdValues[uiCol];
		}
		for (uiRow = 1; uiRow < m_uiN; uiRow++)
		{
			unsigned int uiR_Index = uiRow * m_uiN;
			unsigned int uiRR_Index = uiRow * m_uiN + uiRow;
			// compute L(row)
			for (uiCol = 0; uiCol < uiRow; uiCol++)
			{
				unsigned int uiL_Index = uiR_Index + uiCol;
				
				m_lpdL_Values[uiL_Index] = m_lpdValues[uiL_Index];
				for (uiK = 0; uiK < uiCol; uiK++) // k
				{
					m_lpdL_Values[uiL_Index] -= m_lpdL_Values[uiR_Index + uiK] * m_lpdU_Values[uiK * m_uiN + uiCol];
				}
				m_lpdL_Values[uiL_Index] /= m_lpdU_Values[uiCol * (1 + m_uiN)];
			}
			m_lpdL_Values[uiRR_Index] = 1.0;
			// Compute U(row)
			for (uiCol = uiRow; uiCol < m_uiN; uiCol++)
			{
				unsigned int uiU_Index = uiR_Index + uiCol;
				m_lpdU_Values[uiU_Index] = m_lpdValues[uiU_Index];
				for (uiK = 0; uiK < uiRow; uiK++)
				{
					m_lpdU_Values[uiU_Index] -= m_lpdL_Values[uiR_Index + uiK] * m_lpdU_Values[uiK * m_uiN + uiCol];
				}
			}
		}
		m_bLU_Initialized = true;
	}
}

XVECTOR	xsquare_matrix::Solve_LU(const XVECTOR &i_vB)
{
	XVECTOR	vX;
	Perform_LU_Decomposition();
	if (m_lpdValues && i_vB.m_lpdValues && i_vB.m_uiN == m_uiN && m_bLU_Initialized)
	{
		XVECTOR	vY(m_uiN);
		vY = Forward_Substituion(i_vB);
		vX = Back_Substituion(m_lpdU_Values,vY);
	}
	return vX;
}


XVECTOR * g_lpvX = NULL;
unsigned int g_uiX_Size = 0;

class DUMMY
{
public:
	DUMMY(void){;}
	~DUMMY(void)
	{
		if (g_lpvX)
			delete [] g_lpvX;
		g_lpvX = NULL;
		g_uiX_Size = 0;
	}
};

DUMMY g_cDummy;
void xsquare_matrix::Invert_LUD(void)
{ // Invert the matrix using LU decomposition
	Perform_LU_Decomposition();
	if (m_lpdValues && m_bLU_Initialized)
	{
		if (g_lpvX == NULL || g_uiX_Size < m_uiN)
		{
			if (g_lpvX)
				delete [] g_lpvX;
			g_lpvX = new XVECTOR[m_uiN];
			g_uiX_Size = m_uiN;
		}
		XVECTOR	vB(m_uiN);
        XVECTOR	vY(m_uiN);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
			vB.Set(uiI,0.0);
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
		{
			if (uiI != 0)
				vB.Set(uiI - 1,0.0);
			vB.Set(uiI,1.0);
			vY = Forward_Substituion(vB);
			g_lpvX[uiI] = Back_Substituion(m_lpdU_Values,vY);
		}
		// Inverse found (stored in the array of X vectors)
		// no refill matrix
		for (unsigned int uiI = 0; uiI < m_uiN; uiI++)
			Set(uiI, g_lpvX[uiI]);
		m_bLU_Initialized = false;
		// Get LU decomposition of inverted matrix
		Perform_LU_Decomposition();
	}
}

// print the matrix to file or console
void	xsquare_matrix::Print(const char * lpszFormat, char chSeparator, FILE * i_fileOut, WHICH i_eWhich)
{
	if ((i_eWhich == MAT_U || i_eWhich == MAT_L) && !m_bLU_Initialized)
		Perform_LU_Decomposition();
	double	dVal = 0.0;
	for (unsigned int uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		for (unsigned int uiCol = 0; uiCol < m_uiN; uiCol++)
		{
			switch (i_eWhich)
			{
			default:
			case MAT_A:
				dVal = m_lpdValues[uiRow * m_uiN + uiCol];
				break;
			case MAT_L:
				if (m_lpdL_Values)
					dVal = m_lpdL_Values[uiRow * m_uiN + uiCol];
				break;
			case MAT_U:
				if (m_lpdU_Values)
					dVal = m_lpdU_Values[uiRow * m_uiN + uiCol];
				break;
			}
			if (i_fileOut)
			{
				fprintf(i_fileOut,lpszFormat,dVal);
				fprintf(i_fileOut,"%c",chSeparator);
			}
			else
			{
				printf(lpszFormat,dVal);
				printf("%c",chSeparator);
			}
		}
		if (i_fileOut)
			fprintf(i_fileOut,"\n");
		else
			printf("\n");
	}
}

	// Transpose the matrix
void	xsquare_matrix::Transpose(void)
{
	unsigned int uiRow, uiCol;
	for (uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		for (uiCol = uiRow + 1; uiCol < m_uiN; uiCol++)
		{
			unsigned int uiTop = uiRow * m_uiN + uiCol;
			unsigned int uiBot = uiCol * m_uiN + uiRow;
			double	dSwap = m_lpdValues[uiTop];
			m_lpdValues[uiTop] = m_lpdValues[uiBot];
			m_lpdValues[uiBot] = dSwap;
		}
	}
}

	// Generate rotation matrix, given Euler angles phi (x), theta (y), and psi (z)
void	xsquare_matrix::Create_Rotation_Matrix(const double & i_dPhi_Rad,const double &i_dTheta_Rad, const double & i_dPsi_Rad)
{

	if (m_uiN == 0)
		Set_Size(3);
	
	if (m_uiN == 3)
	{
		double	dCosBeta = cos(i_dTheta_Rad);
		double	dSinBeta = sin(i_dTheta_Rad);
		double	dCosAlpha = cos(i_dPhi_Rad);
		double	dSinAlpha = sin(i_dPhi_Rad);
		double	dCosGamma = cos(i_dPsi_Rad);
		double	dSinGamma = sin(i_dPsi_Rad);

		m_lpdValues[0] = dCosGamma * dCosBeta + dSinAlpha * dSinBeta * dSinGamma;
		m_lpdValues[3] = -dSinGamma * dCosBeta + dSinAlpha * dSinBeta * dCosGamma;
		m_lpdValues[6] = dCosAlpha * dSinBeta;

		m_lpdValues[1] = dCosAlpha * dSinGamma;
		m_lpdValues[4] = dCosAlpha * dCosGamma;
		m_lpdValues[7] = -dSinAlpha;

		m_lpdValues[2] = -dCosGamma * dSinBeta + dSinGamma * dSinAlpha * dCosBeta;
		m_lpdValues[5] = dSinGamma * dSinBeta + dCosGamma * dSinAlpha * dCosBeta;
		m_lpdValues[8] = dCosAlpha * dCosBeta;
	}
				


}


bool xsquare_matrix::is_nan(void) const
{
	bool bRet = false;
	unsigned int uiN = m_uiN * m_uiN;
	for (unsigned int uiI = 0; uiI < uiN && !bRet;  uiI++)
	{
		bRet = isnan(m_lpdValues[uiI]);
	}
	return bRet;
}
bool xsquare_matrix::is_inf(void) const
{
	bool bRet = false;
	unsigned int uiN = m_uiN * m_uiN;
	for (unsigned int uiI = 0; uiI < uiN && !bRet;  uiI++)
	{
		bRet = isinf(m_lpdValues[uiI]);
	}
	return bRet;
}

xvector	xsquare_matrix::Get_Eigenvector(const double & i_dLambda) const
{
	xvector vRet;
	return vRet;
}

