#include <string.h>
#include <xlinalgep.h>


//----------------------------------------------------------------------------
//
// SQUARE_MATRIX class
//
//----------------------------------------------------------------------------
// default constructor
xsquare_matrix_ep::xsquare_matrix_ep(void)
{
	m_lpdValues = nullptr;
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = nullptr;
	m_lpuiPivot_Table = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor sith specified size
xsquare_matrix_ep::xsquare_matrix_ep(size_t i_uiN)
{
	m_lpdValues = new expdouble[i_uiN * i_uiN];
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = nullptr;
	m_lpuiPivot_Table = nullptr;
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xsquare_matrix_ep::xsquare_matrix_ep(const xsquare_matrix_ep &i_cRHO)
{
	m_lpdValues = new expdouble[i_cRHO.m_uiN * i_cRHO.m_uiN];
	m_lpuiPivot_Table = nullptr;
	m_uiN = i_cRHO.m_uiN;
	m_uiN_Alloc = i_cRHO.m_uiN;
	memcpy(m_lpdValues,i_cRHO.m_lpdValues,
		i_cRHO.m_uiN * i_cRHO.m_uiN * sizeof(expdouble));
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = nullptr;
	Perform_LU_Decomposition();
}
// desructor
xsquare_matrix_ep::~xsquare_matrix_ep(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	if (m_lpuiPivot_Table)
		delete [] m_lpuiPivot_Table;
	if (m_lpdL_Values)
		delete [] m_lpdL_Values;
	if (m_lpdU_Values)
		delete [] m_lpdU_Values;
	m_lpdValues = nullptr;
	m_lpuiPivot_Table = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	m_bLU_Initialized = false;
	m_lpdL_Values = m_lpdU_Values = nullptr;
}
// set size of matrix
void	xsquare_matrix_ep::Set_Size(size_t i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpdValues = new expdouble[i_uiN * i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(expdouble) * m_uiN * m_uiN);
}
// set matrix element
void	xsquare_matrix_ep::Set(
			size_t i_uiRow, 
			size_t i_uiCol, 
			const expdouble &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
	{
		m_bLU_Initialized &= (m_lpdValues[i_uiRow * m_uiN + i_uiCol] == i_dValue); // 
		m_lpdValues[i_uiRow * m_uiN + i_uiCol] = i_dValue;
	}
}
// set column vector
void	xsquare_matrix_ep::Set(size_t i_uiCol, const xvector_ep &i_vCol)
{
	if (m_lpdValues && i_uiCol < m_uiN && 
		i_vCol.m_lpdValues && i_vCol.m_uiN == m_uiN)
	{
		for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			m_bLU_Initialized &= (m_lpdValues[uiRow * m_uiN + i_uiCol] == i_vCol.m_lpdValues[uiRow]); // 
			m_lpdValues[uiRow * m_uiN + i_uiCol] = 
				i_vCol.m_lpdValues[uiRow];
		}
	}
}
// add a value to an element
void	xsquare_matrix_ep::Element_Add(
		size_t i_uiRow, 
		size_t i_uiCol, 
		const expdouble &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
	{
		m_bLU_Initialized &= (i_dValue == 0.0); // 
		m_lpdValues[i_uiRow * m_uiN + i_uiCol] += i_dValue;
	}
}
// retrieve an element value
expdouble	xsquare_matrix_ep::Get(size_t i_uiRow, size_t i_uiCol) const
{
	expdouble	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN && i_uiCol < m_uiN)
		dRet =  m_lpdValues[i_uiRow * m_uiN + i_uiCol];
	return dRet;
}
xvector_ep	xsquare_matrix_ep::GetColumn(size_t i_uiCol) const
{
	xvector_ep	vRet;
	size_t uiRow;
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
xsquare_matrix_ep & xsquare_matrix_ep::operator =(const xsquare_matrix_ep &i_cRHO)
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
		m_lpdL_Values = m_lpdU_Values = nullptr;
		m_lpdValues = new expdouble[i_cRHO.m_uiN * i_cRHO.m_uiN];
		m_uiN = m_uiN_Alloc = i_cRHO.m_uiN;
	}
	if (m_uiN_Alloc >= i_cRHO.m_uiN)
	{
		if (m_uiN_Alloc > i_cRHO.m_uiN)
			memset(m_lpdValues,0,m_uiN_Alloc * m_uiN_Alloc * sizeof(expdouble));
		memcpy(m_lpdValues,i_cRHO.m_lpdValues,
			sizeof(expdouble) * i_cRHO.m_uiN * i_cRHO.m_uiN);
	}
	return *this;
}
// matrix multiplication
xsquare_matrix_ep & xsquare_matrix_ep::operator *=(const xsquare_matrix_ep &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN && m_uiN > 0)
	{
		m_bLU_Initialized = false;
		expdouble * lpdI = new expdouble[m_uiN * m_uiN];
		memset(lpdI,0,sizeof(expdouble) * m_uiN * m_uiN); // clear
		for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
			for (size_t uiCol = 0; uiCol < m_uiN; uiCol++)
				for (size_t uiK = 0; uiK < m_uiN; uiK++)
					lpdI[uiRow * m_uiN + uiCol] += 
						m_lpdValues[uiRow * m_uiN + uiK] * 
							i_cRHO.m_lpdValues[uiK * m_uiN + uiCol];
		memcpy(m_lpdValues,lpdI,sizeof(expdouble) * m_uiN * m_uiN);
		delete [] lpdI;
	}
	return *this;
}
// matrix multiplication
xsquare_matrix_ep xsquare_matrix_ep::operator *(const xsquare_matrix_ep &i_cRHO) const
{
	xsquare_matrix_ep cMatrix(*this);
	cMatrix *= i_cRHO;
	return cMatrix;
}
// vector multiplication: matrix A, vector x:  r = Ax
xvector_ep xsquare_matrix_ep::operator * (const xvector_ep &i_cRHO) const
{
	xvector_ep	cRes(i_cRHO.m_uiN);

	if (i_cRHO.m_lpdValues && m_lpdValues && m_uiN == i_cRHO.m_uiN)
	{
		for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			cRes.m_lpdValues[uiRow] = 0.0;
			for (size_t uiCol = 0; uiCol < m_uiN; uiCol++)
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
xsquare_matrix_ep & xsquare_matrix_ep::operator *=(const expdouble &i_dRHO)
{
	if (m_lpdValues)
	{
		size_t uiNN = m_uiN * m_uiN;
		for (size_t uiIdx = 0; uiIdx < uiNN; uiIdx++)
		{
			m_lpdValues[uiIdx] *= i_dRHO;
		}
	}
	return *this;
}
// scalar multiplication
xsquare_matrix_ep xsquare_matrix_ep::operator *(const expdouble &i_dRHO)  const
{
	xsquare_matrix_ep cRet(*this);
	cRet *= i_dRHO;
	return cRet;
}
// scalar multiplication
xsquare_matrix_ep operator * (const expdouble &i_dLHO, const xsquare_matrix_ep &i_vRHO)
{
	return i_vRHO * i_dLHO;
}
	// matrix addition
xsquare_matrix_ep & xsquare_matrix_ep::operator +=(const xsquare_matrix_ep &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN)
	{
		size_t uiNum = m_uiN * m_uiN;
		for (size_t uiI = 0; uiI < uiNum; uiI++)
		{
			m_lpdValues[uiI] += i_cRHO.m_lpdValues[uiI];
		}
		m_bLU_Initialized = false;
	}
	return *this;
}
xsquare_matrix_ep xsquare_matrix_ep::operator +(const xsquare_matrix_ep &i_cRHO) const
{
	xsquare_matrix_ep cMatrixOut = *this;
	cMatrixOut += i_cRHO;
	return cMatrixOut;
}
xsquare_matrix_ep & xsquare_matrix_ep::operator -=(const xsquare_matrix_ep &i_cRHO)
{
	if (m_uiN == i_cRHO.m_uiN)
	{
		size_t uiNum = m_uiN * m_uiN;
		for (size_t uiI = 0; uiI < uiNum; uiI++)
		{
			m_lpdValues[uiI] -= i_cRHO.m_lpdValues[uiI];
		}
		m_bLU_Initialized = false;
	}
	return *this;
}
xsquare_matrix_ep xsquare_matrix_ep::operator -(const xsquare_matrix_ep &i_cRHO)  const
{
	xsquare_matrix_ep cMatrixOut = *this;
	cMatrixOut -= i_cRHO;
	return cMatrixOut;
}

// set the matrix to the idenity matrix
void	xsquare_matrix_ep::Identity(void)
{
	if (m_lpdValues)
	{
		Zero();
		for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			m_lpdValues[uiRow * m_uiN + uiRow] = 1.0;
		}
	}
}
// set all elements to zero
void	xsquare_matrix_ep::Zero(void)
{
	if (m_lpdValues)
	{
		m_bLU_Initialized = false;
		if (m_uiN != m_uiN_Alloc)
		{
			fprintf(stderr,"N != nalloc in sqaure matrix\n");
			fflush(stderr);
		}
		memset(m_lpdValues,0,m_uiN_Alloc * m_uiN_Alloc * sizeof(expdouble));
	}
}

// scale an entire matrix row by a value
void	xsquare_matrix_ep::Scale_Row(size_t i_uiRow, const expdouble &i_dScalar)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_bLU_Initialized &= (i_dScalar == 1.0);
		size_t uiIdx_Ref = m_uiN * i_uiRow;
		for (size_t uiCol = 0; uiCol < m_uiN; uiCol++)
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
void	xsquare_matrix_ep::Add_Rows(
			size_t i_uiRow_LHO, 
			size_t i_uiRow_RHO, 
			const expdouble &i_dRHO_Scalar, 
			bool i_bAssume_RR) 
{
	size_t uiStart_Idx = 0;
	// assume all columns to the right of the diagonal are 0
	if (i_bAssume_RR) 
		uiStart_Idx = i_uiRow_RHO;

	size_t uiIdx_Ref_L = i_uiRow_LHO * m_uiN;
	size_t uiIdx_Ref_R = i_uiRow_RHO * m_uiN;
	for (size_t uiCol = uiStart_Idx; uiCol < m_uiN; uiCol++)
	{
		m_bLU_Initialized &= (i_dRHO_Scalar == 0.0 || m_lpdValues[uiIdx_Ref_R + uiCol] == 0.0);
		m_lpdValues[uiIdx_Ref_L + uiCol] += i_dRHO_Scalar * 
								m_lpdValues[uiIdx_Ref_R + uiCol];
	}
}

// Swap two rows
void	xsquare_matrix_ep::Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2)
{
	if (m_lpdValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		m_bLU_Initialized = false;
		size_t uiSize = sizeof(expdouble) * m_uiN;
		expdouble * lpdTemp = new expdouble [m_uiN];
		memcpy(lpdTemp,&m_lpdValues[i_uiRow_1 * m_uiN],uiSize);
		memcpy(&m_lpdValues[i_uiRow_1 * m_uiN],
					&m_lpdValues[i_uiRow_2 * m_uiN],uiSize);
		memcpy(&m_lpdValues[i_uiRow_2 * m_uiN],lpdTemp,uiSize);
		delete [] lpdTemp;
	}
}

// perform Guass-Jordanian elimination:  The matrix will become
// it's inverse
void	xsquare_matrix_ep::Inverse_GJ(void)
{
	if (m_lpdValues)
	{
		xsquare_matrix_ep	cI(m_uiN);
		cI.Identity();
		// allocate work space
		expdouble	dScalar;

		// Using row reduction, reduce the source matrix to the identity, 
		// and the identity will become A^{-1}
		for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
		{
			if (m_lpdValues[uiRow * m_uiN + uiRow] == 0.0)
			{
				for (size_t uiRowInner = uiRow + 1; 
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
			dScalar = expdouble(1.0) / m_lpdValues[uiRow * m_uiN + uiRow];
			Scale_Row(uiRow,dScalar);
			cI.Scale_Row(uiRow,dScalar);

			for (size_t uiRowInner = 0; 
					uiRowInner < m_uiN; 
					uiRowInner++)
			{
				expdouble	dRow_Scalar = 
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

xvector_ep	xsquare_matrix_ep::Back_Substituion(const expdouble * lpdValue_set, const xvector_ep &i_vB)
{
	xvector_ep vX(m_uiN);
	size_t uiRow;
	size_t uiCol;
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

xvector_ep	xsquare_matrix_ep::Forward_Substituion(const xvector_ep &i_vB)
{
	xvector_ep vX(m_uiN);
	size_t uiRow;
	size_t uiCol;
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
xvector_ep	xsquare_matrix_ep::Solve_GEb(const xvector_ep &i_vB)
{
	xvector_ep	vX;
	xvector_ep	vBlcl = i_vB;
	if (m_lpdValues && i_vB.m_lpdValues && i_vB.m_uiN == m_uiN)
	{
		expdouble * lpdValues_Store = new expdouble [m_uiN * m_uiN];
		// store original matrix
		memcpy(lpdValues_Store,m_lpdValues,
					sizeof(expdouble) * m_uiN * m_uiN);
		vX.Set_Size(m_uiN);

		size_t uiRef_Idx = 0;
		expdouble	dMax = 0.0;
		size_t uiRow, uiRowInner, uiCol;
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
				expdouble	dInner_Scalar = 
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
					sizeof(expdouble) * m_uiN * m_uiN);
		delete [] lpdValues_Store;
	}
	return vX;
}
void xsquare_matrix_ep::Perform_LU_Decomposition(void)
{// Perform LU Decomposition using Crout's method
	size_t uiRow, uiCol, uiK;
	if (!m_bLU_Initialized && m_lpdValues)
	{
		if (!m_lpdL_Values)
			m_lpdL_Values = new expdouble [m_uiN_Alloc * m_uiN_Alloc];
		if (!m_lpdU_Values)
			m_lpdU_Values = new expdouble [m_uiN_Alloc * m_uiN_Alloc];
		if (!m_lpuiPivot_Table)
			m_lpuiPivot_Table = new size_t [m_uiN_Alloc];
		for (uiRow = 0; uiRow < m_uiN_Alloc; uiRow++)
			m_lpuiPivot_Table[uiRow] = uiRow;

		memset(m_lpdU_Values,0,sizeof(expdouble) * m_uiN * m_uiN);
		memset(m_lpdL_Values,0,sizeof(expdouble) * m_uiN * m_uiN);
		m_lpdL_Values[0] = 1.0;
		for (uiCol = 0; uiCol < m_uiN; uiCol++)
		{
			m_lpdU_Values[uiCol] = m_lpdValues[uiCol];
		}
		for (uiRow = 1; uiRow < m_uiN; uiRow++)
		{
			size_t uiR_Index = uiRow * m_uiN;
			size_t uiRR_Index = uiRow * m_uiN + uiRow;
			// compute L(row)
			for (uiCol = 0; uiCol < uiRow; uiCol++)
			{
				size_t uiL_Index = uiR_Index + uiCol;
				
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
				size_t uiU_Index = uiR_Index + uiCol;
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

xvector_ep	xsquare_matrix_ep::Solve_LU(const xvector_ep &i_vB)
{
	xvector_ep	vX;
	Perform_LU_Decomposition();
	if (m_lpdValues && i_vB.m_lpdValues && i_vB.m_uiN == m_uiN && m_bLU_Initialized)
	{
		xvector_ep	vY(m_uiN);
		vY = Forward_Substituion(i_vB);
		vX = Back_Substituion(m_lpdU_Values,vY);
	}
	return vX;
}


xvector_ep * g_xpvlX = nullptr;
size_t g_uiXx_Size = 0;

class dummyxp
{
public:
	dummyxp(void){;}
	~dummyxp(void)
	{
		if (g_xpvlX)
			delete [] g_xpvlX;
		g_xpvlX = nullptr;
		g_uiXx_Size = 0;
	}
};

dummyxp g_cdummyxp;
void xsquare_matrix_ep::Invert_LUD(void)
{ // Invert the matrix using LU decomposition
	Perform_LU_Decomposition();
	if (m_lpdValues && m_bLU_Initialized)
	{
		if (g_xpvlX == nullptr || g_uiXx_Size < m_uiN)
		{
			if (g_xpvlX)
				delete [] g_xpvlX;
			g_xpvlX = new xvector_ep[m_uiN];
			g_uiXx_Size = m_uiN;
		}
		xvector_ep	vB(m_uiN);
        xvector_ep	vY(m_uiN);
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
			vB.Set(uiI,0.0);
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			if (uiI != 0)
				vB.Set(uiI - 1,0.0);
			vB.Set(uiI,1.0);
			vY = Forward_Substituion(vB);
			g_xpvlX[uiI] = Back_Substituion(m_lpdU_Values,vY);
		}
		// Inverse found (stored in the array of X vectors)
		// no refill matrix
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
			Set(uiI, g_xpvlX[uiI]);
		m_bLU_Initialized = false;
		// Get LU decomposition of inverted matrix
		Perform_LU_Decomposition();
	}
}


	// Transpose the matrix
void	xsquare_matrix_ep::Transpose(void)
{
	size_t uiRow, uiCol;
	for (uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		for (uiCol = uiRow + 1; uiCol < m_uiN; uiCol++)
		{
			size_t uiTop = uiRow * m_uiN + uiCol;
			size_t uiBot = uiCol * m_uiN + uiRow;
			expdouble	dSwap = m_lpdValues[uiTop];
			m_lpdValues[uiTop] = m_lpdValues[uiBot];
			m_lpdValues[uiBot] = dSwap;
		}
	}
}

	// Generate rotation matrix, given Euler angles phi (x), theta (y), and psi (z)
void	xsquare_matrix_ep::Create_Rotation_Matrix(const expdouble & i_dPhi_Rad,const expdouble &i_dTheta_Rad, const expdouble & i_dPsi_Rad)
{

	if (m_uiN == 0)
		Set_Size(3);
	
	if (m_uiN == 3)
	{
		expdouble	dCosBeta(std::cos(i_dTheta_Rad.unload()));
		expdouble	dSinBeta(std::sin(i_dTheta_Rad.unload()));
		expdouble	dCosAlpha(std::cos(i_dPhi_Rad.unload()));
		expdouble	dSinAlpha(std::sin(i_dPhi_Rad.unload()));
		expdouble	dCosGamma(std::cos(i_dPsi_Rad.unload()));
		expdouble	dSinGamma(std::sin(i_dPsi_Rad.unload()));

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


bool xsquare_matrix_ep::is_nan(void) const
{
	bool bRet = false;
	size_t uiN = m_uiN * m_uiN;
	for (size_t uiI = 0; uiI < uiN && !bRet;  uiI++)
	{
		bRet = m_lpdValues[uiI].isnan();
	}
	return bRet;
}
bool xsquare_matrix_ep::is_inf(void) const
{
	bool bRet = false;
	size_t uiN = m_uiN * m_uiN;
	for (size_t uiI = 0; uiI < uiN && !bRet;  uiI++)
	{
		bRet = m_lpdValues[uiI].isinf();
	}
	return bRet;
}

xvector_ep	xsquare_matrix_ep::Get_Eigenvector(const expdouble & i_dLambda) const
{
	xvector_ep vRet(m_uiN);
	expdouble *		lpdValues = new expdouble[m_uiN * m_uiN];
	memcpy(lpdValues,m_lpdValues,sizeof(expdouble) * m_uiN * m_uiN);
	// make the matrix right triangular -- since this is an eigenvalue problem, this is done
	// using the relation a_{ii} x_i + Sum(j) a_{ij} x_j = lambda x_i
	// and substituting x_i = 1/(lambda - a_{ii}) (Sum(j) a_{ij} x_j) in each lower row
	for (size_t tRow = 0; tRow < m_uiN - 1; tRow++)
	{
		expdouble dRow_Scalar = expdouble(1.0) / (i_dLambda - lpdValues[tRow * (m_uiN + 1)]);
#pragma omp for //shared(tRow,dRow_Scalar)
		for (size_t tRow_J = tRow + 1; tRow_J < m_uiN - 1; tRow_J++)
		{
			if (lpdValues[tRow_J * m_uiN + tRow] != 0.0)
			{
				expdouble dRow_J_Scalar = lpdValues[tRow_J * m_uiN + tRow] * dRow_Scalar;
				for (size_t tCol = tRow + 1; tCol < m_uiN; tCol++)
				{
					//printf("%i %i: %.2f -> ",tRow_J,tCol,lpdValues[tRow_J * m_uiN + tCol]);
					lpdValues[tRow_J * m_uiN + tCol] += dRow_J_Scalar * lpdValues[tRow * m_uiN + tCol];
					//printf("%.2f\n",lpdValues[tRow_J * m_uiN + tCol]);
				}
				lpdValues[tRow_J * m_uiN + tRow] = 0.0;
			}
		}
	}
	for (size_t tCol = 0; tCol < m_uiN; tCol++)
		lpdValues[(m_uiN - 1) * m_uiN + tCol] = 0.0;
	lpdValues[(m_uiN - 1) * m_uiN + (m_uiN - 1)] = 1.0;
/*	printf("-------------------------------\n");
	for (size_t tRow = 0; tRow < m_uiN; tRow++)
	{
		for (size_t tCol = 0; tCol < m_uiN; tCol++)
		{
			printf("\t%.2f",lpdValues[tRow * m_uiN + tCol]);
		}
		printf("\n");
	}


	printf("-------------------------------\n");*/
	vRet[m_uiN - 1] = 1.0; // we are determining the values of all components relative to the last component
	// determine value of x_{n-1} relative to x_{n}
	for (size_t tRow = m_uiN - 2; tRow < m_uiN; tRow--)
	{
		expdouble dA = i_dLambda - lpdValues[tRow * (m_uiN + 1)]; // 
		lpdValues[tRow * m_uiN + m_uiN - 1] /= dA;
		lpdValues[tRow * (m_uiN + 1)] = 0.0;
#pragma omp for
		for (size_t tRow_J = 0; tRow_J < tRow; tRow_J++)
		{
//			printf("%i %i: %.2f -> ",tRow_J,tRow,lpdValues[tRow_J * m_uiN + m_uiN - 1]);
			lpdValues[tRow_J * m_uiN + m_uiN - 1] += lpdValues[tRow * m_uiN + m_uiN - 1] * lpdValues[tRow_J * m_uiN + tRow];
//			printf("%.2f\n",lpdValues[tRow_J * m_uiN + m_uiN - 1]);
			lpdValues[tRow_J * m_uiN + tRow] = 0.0;
		}
	}
/*	printf("-------------------------------\n");
	for (size_t tRow = 0; tRow < m_uiN; tRow++)
	{
		for (size_t tCol = 0; tCol < m_uiN; tCol++)
		{
			printf("\t%.2f",lpdValues[tRow * m_uiN + tCol]);
		}
		printf("\n");
	}*/
	// place values relative to x_n into vector and sum them
	expdouble dSum(0.0);
	for (size_t tRow = 0; tRow < m_uiN; tRow++)
	{
		vRet[tRow] = lpdValues[tRow * m_uiN + m_uiN - 1];

		dSum += (vRet[tRow]); // we are determining the values of all components relative to the last component
	}
	// normalize vector such that Sum (x_i) = 1
	expdouble dScalar = expdouble(1.0) / dSum;
#pragma omp for //shared(dScalar)
	for (size_t tRow = 0; tRow < m_uiN; tRow++)
		vRet[tRow] *= dScalar;
	return vRet;
}

