#include <string.h>
#include <xlinalg.h>

// class containing an n-vector
// default constructor
xvector_long::xvector_long(void)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
}
// constructor specifying size only
xvector_long::xvector_long(size_t i_uiN)
{
	m_lpdValues = new long double[i_uiN];
	m_uiN = i_uiN;
	m_uiN_Alloc = i_uiN;
}
// copy constructor
xvector_long::xvector_long(const xvector_long &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_long::xvector_long(const std::vector<long double> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_long::xvector_long(const std::vector<double> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_long::xvector_long(const std::vector<float> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
// copy constructor
xvector_long::xvector_long(const std::vector<int64_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<unsigned int64_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<int32_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<unsigned int32_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<int16_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<unsigned int16_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<int8_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
xvector_long::xvector_long(const std::vector<unsigned int8_t> &i_vRHO)
{
	m_lpdValues = nullptr;
	m_uiN = 0;
	m_uiN_Alloc = 0;
	Copy(i_vRHO);
}
	// destructor
xvector_long::~xvector_long(void)
{
	if (m_lpdValues)
		delete [] m_lpdValues;
	m_lpdValues = nullptr;
	m_uiN_Alloc = 0;
	m_uiN = 0;
}

// set size of vector
void	xvector_long::Set_Size(size_t i_uiN)
{
	if (i_uiN >= m_uiN_Alloc)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_uiN_Alloc = m_uiN = 0;
		m_lpdValues = new long double[i_uiN];
		m_uiN_Alloc = m_uiN = i_uiN;
	}
	else
		m_uiN = i_uiN;
	memset(m_lpdValues,0,sizeof(long double) * m_uiN);
}
// set a vector element
void	xvector_long::Set(size_t i_uiRow, const long double &i_dValue)
{
	if (m_lpdValues && i_uiRow < m_uiN)
	{
		m_lpdValues[i_uiRow] = i_dValue;
	}
}
// get a vector element
long double	xvector_long::Get(size_t i_uiRow) const
{
	long double	dRet = 0.0;
	if (m_lpdValues && i_uiRow < m_uiN)
		dRet =  m_lpdValues[i_uiRow];
	return dRet;
}
// swap elements
void	xvector_long::Swap_Rows(size_t i_uiRow_1, size_t i_uiRow_2)
{
	long double	dTemp;
	if (m_lpdValues && i_uiRow_1 < m_uiN && i_uiRow_2 < m_uiN)
	{
		dTemp = m_lpdValues[i_uiRow_1];
		m_lpdValues[i_uiRow_1] = m_lpdValues[i_uiRow_2];
		m_lpdValues[i_uiRow_2] = dTemp;
	}
}
// print the matrix to file or console
void	xvector_long::Print(FILE * fileOut) const
{
	for (size_t uiRow = 0; uiRow < m_uiN; uiRow++)
	{
		if (!fileOut)
			fileOut = stdout;
		fprintf(fileOut,"%u,%Lf\n",uiRow,m_lpdValues[uiRow]);
		fflush(fileOut);
	}
}

// Perform dot product
long double	xvector_long::Dot(const xvector_long & i_vRHO) const
{
	long double dRet = 0.0;
	if (m_uiN == i_vRHO.m_uiN)
	{

#pragma omp parallel for reduction(+:dRet)
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
			dRet += m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
	}
	return dRet;
}

// Perform Cross product
xvector_long xvector_long::Cross3d(const xvector_long & i_vRHO) const
{
	xvector_long vRet;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 3)
	{
		vRet.Set_Size(3);
		vRet.m_lpdValues[0] =   m_lpdValues[1] * i_vRHO.m_lpdValues[2] - m_lpdValues[2] * i_vRHO.m_lpdValues[1];
		vRet.m_lpdValues[1] = - m_lpdValues[0] * i_vRHO.m_lpdValues[2] + m_lpdValues[2] * i_vRHO.m_lpdValues[0];
		vRet.m_lpdValues[2] =   m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return vRet;
}

long double xvector_long::Cross2d(const xvector_long & i_vRHO) const
{
	long double	dRet=0.0;
	if (m_uiN == i_vRHO.m_uiN && m_uiN == 2)
	{
		dRet = m_lpdValues[0] * i_vRHO.m_lpdValues[1] - m_lpdValues[1] * i_vRHO.m_lpdValues[0];
	}
	return dRet;
}

/*
XTENSOR	xvector_long::Cross(const xvector_long & i_vRHO) const
{
	XTENSOR	tRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		// TBD
	}
	return tRet;
}
*/

// Aritmetic operations
xvector_long &xvector_long::operator =(const xvector_long &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xvector_long &xvector_long::operator =(const std::vector<long double> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<double> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<float> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<unsigned int64_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<int64_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<unsigned int32_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<int32_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<unsigned int16_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<int16_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<unsigned int8_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}
xvector_long &xvector_long::operator =(const std::vector<int8_t> &i_vRHO)
{
	Copy(i_vRHO);
	return *this;
}

xvector_long xvector_long::operator +(const xvector_long &i_vRHO) const
{
	xvector_long	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] + i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
xvector_long &xvector_long::operator +=(const xvector_long &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] += i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}
xvector_long xvector_long::operator -(const xvector_long &i_vRHO) const
{
	xvector_long	vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] - i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}
xvector_long &xvector_long::operator -=(const xvector_long &i_vRHO)
{
	if (m_uiN == i_vRHO.m_uiN)
	{
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] -= i_vRHO.m_lpdValues[uiI];
		}
	}
	return *this;
}
// Scalar operatations
xvector_long xvector_long::operator -(void)  const
{
	xvector_long	vRet(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = -m_lpdValues[uiI];
	}
	return vRet;
}
xvector_long xvector_long::operator *(const long double &i_dRHO)  const
{
	xvector_long	vRet(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_dRHO;
	}
	return vRet;
}

xvector_long &xvector_long::operator *=(const long double &i_dRHO)
{
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= i_dRHO;
	}
	return *this;
}
xvector_long xvector_long::operator /(const long double &i_dRHO)  const
{
	long double	d1_RHO = 1. / i_dRHO;
	xvector_long	vRet(m_uiN);
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * d1_RHO;
	}
	return vRet;
}
xvector_long &xvector_long::operator /=(const long double &i_dRHO)
{
	long double	d1_RHO = 1.0 / i_dRHO;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		m_lpdValues[uiI] *= d1_RHO;
	}
	return *this;
}
// Boolean operations
bool	xvector_long::operator==(const xvector_long &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return bRet;
}
bool	xvector_long::operator!=(const xvector_long &i_vRHO)  const
{
	bool bRet = m_uiN == i_vRHO.m_uiN;
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
	{
		bRet &= (m_lpdValues[uiI] == i_vRHO.m_lpdValues[uiI]);
	}
	return !bRet;
}

void xvector_long::Copy(const xvector_long &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.m_uiN)
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.m_uiN];
		m_uiN_Alloc = i_vRHO.m_uiN;
	}
	m_uiN = i_vRHO.m_uiN;
	memcpy(m_lpdValues,i_vRHO.m_lpdValues, i_vRHO.m_uiN * sizeof(long double));
}

void xvector_long::Copy(const std::vector<long double> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<double> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<float> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<unsigned int64_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<int64_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}

void xvector_long::Copy(const std::vector<unsigned int32_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<int32_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<unsigned int16_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<int16_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}

void xvector_long::Copy(const std::vector<unsigned int8_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}
void xvector_long::Copy(const std::vector<int8_t> &i_vRHO)
{
	if (m_uiN_Alloc < i_vRHO.size())
	{
		if (m_lpdValues)
			delete [] m_lpdValues;
		m_lpdValues = new long double[i_vRHO.size()];
		m_uiN_Alloc = i_vRHO.size();
	}
	m_uiN = i_vRHO.size();
#pragma omp for
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		m_lpdValues[uiI] = i_vRHO[uiI];

}



xvector_long operator * (const long double &i_dLHO, const xvector_long &i_vRHO)
{
	return i_vRHO * i_dLHO;
}

// Vector multiply: x_i = l_i * r_i
xvector_long	xvector_long::Vector_Multiply(const xvector_long & i_vRHO) const
{
	xvector_long vRet;
	if (m_uiN == i_vRHO.m_uiN)
	{
		vRet.Set_Size(m_uiN);
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			vRet.m_lpdValues[uiI] = m_lpdValues[uiI] * i_vRHO.m_lpdValues[uiI];
		}
	}
	return vRet;
}

// Normalize the vector
void xvector_long::Normalize(void)
{
	long double dMag = Magnitude();
	if (dMag != 0.0)
	{
		long double	dNorm = 1.0 / dMag;
#pragma omp for
		for (size_t uiI = 0; uiI < m_uiN; uiI++)
		{
			m_lpdValues[uiI] *= dNorm;
		}
	}
}
// Set all elements to zero
void xvector_long::Zero(void)
{
	if (m_lpdValues)
	{
		memset(m_lpdValues,0,m_uiN_Alloc * sizeof(long double));
	}
}

// Get the magnitude of a vector
long double	xvector_long::Magnitude(void) const
{
	long double	dMagSqr = Dot(*this);
	return sqrt(dMagSqr);
}

bool xvector_long::is_nan(void) const
{
	bool bRet = false;
#pragma omp parallel for reduction(||:bRet)
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		bRet |= isnan(m_lpdValues[uiI]);
	return bRet;
}

bool xvector_long::is_inf(void) const
{
	bool bRet = false;
#pragma omp parallel for reduction(||:bRet) 
	for (size_t uiI = 0; uiI < m_uiN; uiI++)
		bRet |= isinf(m_lpdValues[uiI]);
	return bRet;
}

